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

#include "ImgSub.h"
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/attributes/SubscriberAttributes.h>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>


using namespace eprosima::fastdds::dds;
using namespace app;

ImgSub::ImgSub(const app::CfgSubImgPtr cfg)
    : cfg_(cfg)
    , participant_(nullptr)
    , subscriber_(nullptr)
    , topic_(nullptr)
    , reader_(nullptr)
    , type_(new MsgImgPubSubType())
    , listener_( cfg )
{
}

bool ImgSub::init(
        bool use_env)
{
    DomainParticipantQos pqos = PARTICIPANT_QOS_DEFAULT;
    //pqos.name( "ImgSub");
    pqos.name( cfg_->pqosName.c_str() );
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

    //REGISTER THE TYPE
    type_.register_type(participant_);

    //CREATE THE SUBSCRIBER
    SubscriberQos sqos = SUBSCRIBER_QOS_DEFAULT;

    if (use_env)
    {
        participant_->get_default_subscriber_qos(sqos);
    }

    subscriber_ = participant_->create_subscriber(sqos, nullptr);

    if (subscriber_ == nullptr)
    {
        return false;
    }

    //CREATE THE TOPIC
    TopicQos tqos = TOPIC_QOS_DEFAULT;

    if (use_env)
    {
        participant_->get_default_topic_qos(tqos);
    }

    topic_ = participant_->create_topic(
        // "ImageTopic",
        // "MsgImg",
        cfg_->topicName.c_str(),
        cfg_->typeName.c_str(),
        tqos);

    if (topic_ == nullptr)
    {
        return false;
    }

    // CREATE THE READER
    DataReaderQos rqos = DATAREADER_QOS_DEFAULT;
    rqos.reliability().kind = RELIABLE_RELIABILITY_QOS;

    if (use_env)
    {
        subscriber_->get_default_datareader_qos(rqos);
    }

    reader_ = subscriber_->create_datareader(topic_, rqos, &listener_);

    if (reader_ == nullptr)
    {
        return false;
    }

    return true;
}

ImgSub::~ImgSub()
{
    if (reader_ != nullptr)
    {
        subscriber_->delete_datareader(reader_);
    }
    if (topic_ != nullptr)
    {
        participant_->delete_topic(topic_);
    }
    if (subscriber_ != nullptr)
    {
        participant_->delete_subscriber(subscriber_);
    }
    DomainParticipantFactory::get_instance()->delete_participant(participant_);
}


void ImgSub::run()
{
  APP_LOG("subscriber running. Please press enter to stop the Subscriber");
  std::cin.ignore();
}

void ImgSub::run(uint32_t number)
{
    std::cout << "Subscriber running until " << number << "samples have been received" << std::endl;
    while (number > listener_.nMsgRcvd_)
    {
      APP_LOG("ImgSub::run(): sleep 500 ms");
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void createImageSubscriber(bool use_environment_qos, const app::CfgSubImgPtr cfg) {
    ImgSub imageSubscriber(cfg);
    if (imageSubscriber.init(use_environment_qos)) imageSubscriber.run();
}

void SubListener::on_subscription_matched(
        DataReader*,
        const SubscriptionMatchedStatus& info)
{
    if (info.current_count_change == 1)
    {
        matched_ = info.total_count;
        std::cout << "Subscriber matched." << std::endl;
    }
    else if (info.current_count_change == -1)
    {
        matched_ = info.total_count;
        std::cout << "Subscriber unmatched." << std::endl;
    }
    else
    {
        std::cout << info.current_count_change
                  << " is not a valid value for SubscriptionMatchedStatus current count change" << std::endl;
    }
}

void SubListener::on_data_available(  DataReader* reader)
{
    SampleInfo info;
    if (reader->take_next_sample(&imgMsg_, &info) == ReturnCode_t::RETCODE_OK)
    {
        if (info.instance_state == ALIVE_INSTANCE_STATE)
        {
            imgMsg_.timeRx_us(APP_TIME_CURRENT_US);
            pktRecved_Byte_ += MsgImg::getCdrSerializedSize( imgMsg_);

            app_time_us_t ddsLatency =  imgMsg_.timeRx_us() - imgMsg_.timeTx_us();
            ddsLatencyStatistics_.addSample( (float)ddsLatency );
            nMsgRcvd_++;

            if( prevFrmNum_ > 0 )
            {
               if( imgMsg_.frmNum() - prevFrmNum_ > 1 )
               {
                    totalLostFrms_ += imgMsg_.frmNum() - prevFrmNum_ - 1;
               }     
            }
            prevFrmNum_ = imgMsg_.frmNum();


            if( 0==nMsgRcvd_%1000){
                ddsLatencyStatistics_.calMeanStd();
                ddsLatencyStatistics_.reset();

                double dt_sec = (double)(APP_TIME_CURRENT_MS - pktStartTime_ms_)/1000.0;
                pktAveragedRate_bps_ = (uint64_t)( (double)(pktRecved_Byte_ * 8)/dt_sec);  //bit/sec
                pktAveragedFrmsPerSec_ = (float)( (double)(nMsgRcvd_ -  pktStartFrmNum_)/dt_sec );

                pktRecved_Byte_ = 0;
                pktStartTime_ms_ = APP_TIME_CURRENT_MS;
                pktStartFrmNum_ = nMsgRcvd_;

                APP_LOG( "fn=%lu, w=%d,h=%d, ddsLatency(%.1f,%.1f)(us), pktAveragedRate_bps_=%lu, pktAveragedFrmsPerSec_=%.1f, totalLostFrms_=%lu\n", 
                imgMsg_.frmNum(), imgMsg_.width(), imgMsg_.height(), 
                ddsLatencyStatistics_.mu, ddsLatencyStatistics_.std, pktAveragedRate_bps_, pktAveragedFrmsPerSec_, totalLostFrms_ );

                //APP_LOG( "fn=%lu, timeAcq=%lu(us), timeTx=%lu(us), w=%d,h=%d, imgMsgSz=%u\n", 
                //imgMsg_.frmNum(), imgMsg_.timeAcq_us(), imgMsg_.timeTx_us(), imgMsg_.width(), imgMsg_.height(), MsgImg::getCdrSerializedSize( imgMsg_) );
            }

            if(dispRcvdImg_){
#if IMG_MSG_FMT_YUV
                cv::Mat frame(imgMsg_.height(), imgMsg_.width(), CV_8UC3);
                app::HostYuvFrm yuv(imgMsg_.width(), imgMsg_.height(), imgMsg_.data().data(), imgMsg_.size(), imgMsg_.frmNum());
                yuv.hdCopyToBgr(frame);
#else
                cv::Mat frame = app::vecToMatSoftCopy(imgMsg_.data(), imgMsg_.width(), imgMsg_.height());
#endif
                cv::putText(frame,
                            app::num_to_string(ddsLatency / 1000, "Rx: ddsLatency: (curr=") +
                                app::num_to_string((int32_t)round(ddsLatencyStatistics_.mu / 1000), ", mean=") +
                                app::num_to_string((int32_t)round(ddsLatencyStatistics_.std / 1000), ", std=") + ")(ms)",
                            cv::Point(10, 80), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(255, 165, 0), 3, false);

                cv::putText(frame,
                            app::num_to_string_format_w_commas<uint64_t>(pktAveragedRate_bps_, "Rx: averaged pkt rate=") + "(bit/sec)",
                            cv::Point(10, 110), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(255, 165, 0), 3, false);

                cv::putText(frame,
                            app::num_to_string(pktAveragedFrmsPerSec_, "Rx: averaged frames=", 2) + "(frm/sec)" +
                                app::num_to_string_format_w_commas<uint64_t>(totalLostFrms_, ", TotalLostFrms="),
                            cv::Point(10, 140), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(255, 165, 0), 3, false);

                cv::imshow(cfg_->dispWindowName, frame);
                cv::waitKey(1);
            }
        }
    }
}
