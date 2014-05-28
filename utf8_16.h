// Utf8_16.h

	#ifndef UTF8_16_H
	
	#pragma once
	
	#ifndef PARAMETERS_H
	#include "Parameters.h"
	#endif// PARAMETERS_H
	
	#ifdef _MSC_VER
	#pragma warning(disable: 4514) // nreferenced inline function has been removed
	#endif
	
	class Utf8_16 {
	public:
	        typedef unsigned short utf16; // 16 bits
	        typedef UCHAR utf8; // 8 bits
	        typedef UCHAR ubyte;
	        static const utf8 k_Boms[uniEnd][3];
	};
	
	// Reads UTF-16 and outputs UTF-8
	class Utf16_Iter : public Utf8_16 {
	public:
	        enum eState {
	            eStart,
	            e2Bytes2,
	            e3Bytes2,
	            e3Bytes3
	        };
	
	        Utf16_Iter();
	        void reset();
	        void set(const ubyte* pBuf, size_t nLen, UniMode eEncoding);
        utf8 get() const { return m_nCur; };
	        void operator++();
	        eState getState() { return m_eState; };
	        operator bool() { return m_pRead <= m_pEnd; };
	
	protected:
	        void toStart(); // Put to start state, swap bytes if necessary
	
	protected:
	        UniMode m_eEncoding;
	        eState m_eState;
	        utf8 m_nCur;
	        utf16 m_nCur16;
	        const ubyte* m_pBuf;
	        const ubyte* m_pRead;
	        const ubyte* m_pEnd;
	};
	
	// Reads UTF-8 and outputs UTF-16
	class Utf8_Iter : public Utf8_16 {
	public:
      Utf8_Iter();
	        void reset();
	        void set(const ubyte* pBuf, size_t nLen, UniMode eEncoding);
	        utf16 get() const {
	#ifdef _DEBUG
                assert(m_eState == eStart);
	#endif
	                return m_nCur;
	        }
        bool canGet() const { return m_eState == eStart; }
	        void operator++();
	        operator bool() { return m_pRead <= m_pEnd; }
	
	protected:
	        void swap();
	        void toStart(); // Put to start state, swap bytes if necessary
	        enum eState {
	            eStart,
	            e2Bytes_Byte2,
	            e3Bytes_Byte2,
	            e3Bytes_Byte3
	        };
	protected:
	        UniMode m_eEncoding;
	        eState m_eState;
	        utf16 m_nCur;
	        const ubyte* m_pBuf;
	        const ubyte* m_pRead;
	        const ubyte* m_pEnd;
	};
	
	// Reads UTF16 and outputs UTF8
	enum u78 {utf8NoBOM=0, ascii7bits=1, ascii8bits=2};
	class Utf8_16_Read : public Utf8_16 {
	public:
	        Utf8_16_Read();
	        ~Utf8_16_Read();
	
	        size_t convert(char* buf, size_t len);
	        char* getNewBuf() { return reinterpret_cast<char *>(m_pNewBuf); }
	
	        UniMode getEncoding() const { return m_eEncoding; }
	        size_t calcCurPos(size_t pos);
	    static UniMode determineEncoding(const unsigned char *buf, int bufLen);

	protected:
	        void determineEncoding();
	
	        u78 utf8_7bits_8bits();
	private:
	        UniMode    m_eEncoding;
	        ubyte*          m_pBuf;
	        ubyte*          m_pNewBuf;
	        size_t          m_nBufSize;
	        size_t                  m_nSkip;
	        bool            m_bFirstRead;
	        size_t          m_nLen;
	        Utf16_Iter      m_Iter16;
	};
	
	// Read in a UTF-8 buffer and write out to UTF-16 or UTF-8
	class Utf8_16_Write : public Utf8_16 {
	public:
        Utf8_16_Write();
	        ~Utf8_16_Write();
	
	        void setEncoding(UniMode eType);
	
	        FILE * fopen(const TCHAR *_name, const TCHAR *_type);
	        size_t fwrite(const void* p, size_t _size);
	        void   fclose();
	
	        size_t convert(char* p, size_t _size);
	        char* getNewBuf() { return reinterpret_cast<char*>(m_pNewBuf); }
	        size_t calcCurPos(size_t pos);
	
	protected:
	        UniMode m_eEncoding;
	        FILE* m_pFile;
	        ubyte* m_pNewBuf;
	        size_t m_nBufSize;
	        bool m_bFirstWrite;
	};
	
	#endif// UTF8_16_H
