#ifndef _UINT8_STREAM_H_
#define _UINT8_STREAM_H_

#include <assert.h>
#include <fstream>
#include <iostream>
#include <cstring>
#include <math.h>
#include <stdio.h>
#if _WIN32
#include <conio.h>
#endif

#include <string>
#include <vector>
#include "CRC.h"
namespace app {
	class Uint8Stream {
	public:
		Uint8Stream(const uint32_t capacity = 1024);

		~Uint8Stream();

		uint16_t  getLength() const;
		uint16_t  getChecksum() const;
		uint8_t*  getData(const uint32_t offset = 0);
		uint32_t	size() const; //get the size # 
		uint32_t	capacity() const; //get the size # 

		void	resetWrite();
		//push all kind of data at the end of $m_buf$
		void write(const int8_t byte, const bool updateCrc = false);
		void write(const uint8_t byte, const bool updateCrc = false);
		void write(const uint8_t *p, const uint32_t len, const bool updateCrc = false);
		void write(const int16_t x, const bool updateCrc = false);
		void write(const int32_t x, const bool updateCrc = false);
		void write(const uint16_t x, const bool updateCrc = false);
		void write(const uint32_t x, const bool updateCrc = false);
		void write(const uint64_t x, const bool updateCrc = false);
		void write(const float x, const bool updateCrc = false);
		void write(const double x, const bool updateCrc = false);
		void write(const std::string &s, const bool updateCrc = false);

		//overitten 4 bytes starting at $m_buf[loc]$, do not change crc
		void writeAt(const uint32_t loc, const uint32_t value);

		void resetRead();
		bool read(int8_t &byte);
		bool read(uint8_t &byte);
		bool read(uint8_t *p, const uint32_t len);
		bool read(int16_t &x);
		bool read(int32_t &x);
		bool read(uint16_t &x);
		bool read(uint32_t &x);
		bool read(uint64_t &x);
		bool read(float& x);
		bool read(double& x);
		bool read(std::string &x);

		void loadBin(const char* fileName);
		void dumpBin(const char* fileName) const;

		void dumpTxt(const char* fileName=0) const;

	protected:
		void creatBuf();
		void deleteBuf();
		void extendBuf(const uint32_t nAddtionalBytes);

	protected:
		uint32_t m_size;			//size of <m_buf>
		uint32_t m_capacity;      //capacity of <m_buf>
		uint8_t* m_buf;			//the buffer holding data
		uint8_t* m_nextWrite;		//the pointer points to the next unit to write 
		uint32_t m_readCnt;

		CRC16  m_crc;
	};
}
#endif

