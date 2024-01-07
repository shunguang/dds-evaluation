// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "ImgPub.h"
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/attributes/PublisherAttributes.h>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/qos/PublisherQos.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/qos/DataWriterQos.hpp>

#include <thread>
#include <iostream>

using namespace app;
using namespace eprosima::fastdds::dds;

ImgPub::ImgPub(CfgPubImgPtr cfg)
    : cfg_(cfg)
#if IMG_MSG_FMT_YUV    
    , yuv2Pub_(cfg->imgSz.w, cfg->imgSz.h)
#endif    
    , img2Pub_(cfg->imgSz.h, cfg->imgSz.w, CV_8UC3)
    , participant_(nullptr), publisher_(nullptr), topic_(nullptr), writer_(nullptr), type_(new MsgImgPubSubType()), stop_(false)
{
    if ( !fileExists(cfg->imgUrl) ){
        APP_EXIT("video file does not exist: %s", cfg->imgUrl.c_str() );
    }

    cap_ = std::make_shared<CapImg>(cfg->imgUrl);
}

bool ImgPub::init(bool use_env)
{

    imgMsg_.frmNum(0);

    DomainParticipantQos pqos = PARTICIPANT_QOS_DEFAULT;
    pqos.name( cfg_->pqosName.c_str() );
    //pqos.name( "ImgPub" );
    auto factory = DomainParticipantFactory::get_instance();

    if (use_env)
    {
        factory->load_profiles();
        factory->get_default_participant_qos(pqos);
    }

    participant_ = factory->create_participant(0, pqos);

    if (participant_ == nullptr)
    {
        return false;
    }

    // REGISTER THE TYPE
    type_.register_type(participant_);

    // CREATE THE PUBLISHER
    PublisherQos pubqos = PUBLISHER_QOS_DEFAULT;

    if (use_env)
    {
        participant_->get_default_publisher_qos(pubqos);
    }

    publisher_ = participant_->create_publisher(
        pubqos,
        nullptr);

    if (publisher_ == nullptr)
    {
        return false;
    }

    // CREATE THE TOPIC
    TopicQos tqos = TOPIC_QOS_DEFAULT;

    if (use_env)
    {
        participant_->get_default_topic_qos(tqos);
    }

    topic_ = participant_->create_topic(
        //"ImageTopic",
        //"MsgImg",
        cfg_->topicName.c_str(),
        cfg_->typeName.c_str(),
        tqos);

    if (topic_ == nullptr)
    {
        return false;
    }

    // CREATE THE WRITER
    DataWriterQos wqos = DATAWRITER_QOS_DEFAULT;

    if (use_env)
    {
        publisher_->get_default_datawriter_qos(wqos);
    }

    writer_ = publisher_->create_datawriter(
        topic_,
        wqos,
        &listener_);

    if (writer_ == nullptr)
    {
        return false;
    }

    return true;
}

ImgPub::~ImgPub()
{
    if (writer_ != nullptr)
    {
        publisher_->delete_datawriter(writer_);
    }
    if (publisher_ != nullptr)
    {
        participant_->delete_publisher(publisher_);
    }
    if (topic_ != nullptr)
    {
        participant_->delete_topic(topic_);
    }
    DomainParticipantFactory::get_instance()->delete_participant(participant_);
}

void ImgPub::PubListener::on_publication_matched(
    eprosima::fastdds::dds::DataWriter *,
    const eprosima::fastdds::dds::PublicationMatchedStatus &info)
{
    if (info.current_count_change == 1)
    {
        matched_ = info.total_count;
        firstConnected_ = true;
        std::cout << "Publisher matched." << std::endl;
    }
    else if (info.current_count_change == -1)
    {
        matched_ = info.total_count;
        std::cout << "Publisher unmatched." << std::endl;
    }
    else
    {
        std::cout << info.current_count_change
                  << " is not a valid value for PublicationMatchedStatus current count change" << std::endl;
    }
}

void ImgPub::runLoop()
{
    const uint64_t nTotFrames = (0 == cfg_->nTotFrames) ? APP_NAN_UINT64 : cfg_->nTotFrames;
    const int w = cfg_->imgSz.w;
    const int h = cfg_->imgSz.h;

    // there are 1,000,000,000 nanoseconds in a second
    std::chrono::nanoseconds nanoseconds_per_msg = std::chrono::nanoseconds(1000000000 * cfg_->fps.den / cfg_->fps.num);
    APP_LOG("start sending out images: nTotFrames=%ul, nanoseconds_per_msg=%ul", nTotFrames, nanoseconds_per_msg.count());

    sndStartTime_ms_ = APP_TIME_CURRENT_MS;
    for (uint64_t fn = 0; fn < nTotFrames; fn++)
    {
        auto t1 = APP_TIME_CURRENT_NS;
        getNewFrm( fn );
      	//app::myImgWrite(  img2Pub_, ".", "tmp", fn);
        prepareImgMsg(fn);
        if (!publish(false))
        {
            APP_LOG("cannot pub imgMsg fn=%ul", fn);
        }
        auto t2 = APP_TIME_CURRENT_NS;
        auto dt = t2 -t1;
        if( dt < nanoseconds_per_msg.count() ){
            std::chrono::nanoseconds T0( nanoseconds_per_msg.count() - dt  );
            //if( 0 == (fn % 300) )
            //    printf("fn=%lu, dt=%ld, T0=%ld(ms)\n", fn, dt/1000000, T0.count()/1000000 );
            std::this_thread::sleep_for(  T0 );
        }
        else{
            APP_LOG("ImgPub::runLoop(): it take too much time to prepare image msg!");
        }

        if( 0 == (fn % 300) ){
            double dt_sec = (double)(APP_TIME_CURRENT_MS - sndStartTime_ms_)/1000.0;
            sndAveragedFrms_ = (double)fn / dt_sec;
            printf("fn=%lu, dt=%.3f(sec), sndFrms=%.2f (frm/sec)\n", fn, dt_sec, sndAveragedFrms_ );
        }

    }
    APP_LOG("finished sending  out images: sent out nTotFrames=%ul", nTotFrames);
}

#if 0
std::thread ImgPub::run()
{
    stop_ = false;
    std::thread thread(&ImgPub::runLoop, this);
    return thread;
}
#endif

void ImgPub::run()
{

    uint32_t samples = 0;
    stop_ = false;
    std::thread thread(&ImgPub::runLoop, this);
    if (samples == 0)
    {
        std::cout << "Publisher running. Please press enter to stop the Publisher at any time." << std::endl;
        std::cin.ignore();
        stop_ = true;
    }
    else
    {
        APP_LOG("Publisher will run %d samples!", samples);
    }
    thread.join();
}

bool ImgPub::publish(bool waitForListener)
{
    if (listener_.firstConnected_ || !waitForListener || listener_.matched_ > 0)
    {
        writer_->write(&imgMsg_);
        return true;
    }
    return false;
}

bool ImgPub::prepareImgMsg(const uint64_t fn)
{
    const int w = img2Pub_.cols;
    const int h = img2Pub_.rows;
    
    APP_ASSERT( w == cfg_->imgSz.w, "img width is incorrect" );
    APP_ASSERT( h == cfg_->imgSz.h, "img height is incorrect");

    imgMsg_.timeAcq_us(APP_TIME_CURRENT_US);

    imgMsg_.width(w);
    imgMsg_.height(h);
    imgMsg_.frmNum(fn);

    imgMsg_.encType(0);

#if IMG_MSG_FMT_YUV    
    imgMsg_.cap(yuv2Pub_.sz_);
    imgMsg_.size(yuv2Pub_.sz_);
    imgMsg_.data().resize(yuv2Pub_.sz_);
    yuv2Pub_.hdCopyFromBgr(img2Pub_, fn, 0);
    yuv2Pub_.hdCopyTo(imgMsg_.data().data(), yuv2Pub_.sz_);

#else
    imgMsg_.cap(w * h * 3);
    imgMsg_.size(w * h * 3);
    app::matToVec(img2Pub_, imgMsg_.data());
#endif

    imgMsg_.timeTx_us(APP_TIME_CURRENT_US);
    imgMsg_.timeRx_us(0);

    //debug
#if 0    
    cv::Mat I1 = app::vecToMatHardCopy( imgMsg_.data(), w, h);
    cv::Mat I2 = app::vecToMatSoftCopy( imgMsg_.data(), w, h);
    app::myImgWrite(  I1, ".", "tmp1", fn);
    app::myImgWrite(  I2, ".", "tmp2", fn);
#endif

    return true;
}

bool ImgPub::getNewFrm(const uint64_t fn)
{
    bool hasNewFrame = cap_->getNextFrame( img2Pub_ );
    if( !hasNewFrame ){
        cap_->rewind();
        hasNewFrame = cap_->getNextFrame( img2Pub_ );
        APP_LOG( "video rewind!" );
    }

    APP_ASSERT(hasNewFrame, "sth is wrong!");

    const int w = img2Pub_.cols;
    const int h = img2Pub_.rows;
    if( w != cfg_->imgSz.w || h != cfg_->imgSz.h ){
        APP_LOG("todo: resize the imgage before pub captured(%d,%d), pub(%d,%d)", w, h, cfg_->imgSz.w , cfg_->imgSz.h);
    }

    cv::putText(img2Pub_, 
    num_to_string_format_w_commas(fn, "Tx: fn=") + num_to_string(sndAveragedFrms_, ", fps=", 2) + "frm/sec",  
    cv::Point(10,50),cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(255,165,0), 3, false);
 

}
