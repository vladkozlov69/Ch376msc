/*
 * Ch376msc.h
 *
 *  Created on: Feb 25, 2019
 *      Author: György Kovács
 *  v1.2 Apr 24, 2019
 *  	-fixing timing issue on higher SPI clock
 *  	 datasheet 7.3 Time Sequence table (TSC)
 ******************************************************
 *  v1.2 Apr 20, 2019
 *  	-extended with SPI communication
 ******************************************************
 *	v1.1 Feb 25, 2019
 *		-initial version with UART communication
 ******************************************************
 */

#ifndef Ch376msc_H_
#define Ch376msc_H_

#include <Arduino.h>
#include "CommDef.h"
#include <Stream.h>
#include <SPI.h>


#define TIMEOUT 2000 // waiting for data from CH
#define SPICLKRATE 125000 //Clock rate 125kHz
						// max 8000000 (8MHz)on UNO, Mega (16 000 000 / 2 = 8 000 000)

class Ch376msc {
public:
	/////////////Constructors////////////////////////
	Ch376msc(HardwareSerial &usb, uint32_t speed);//HW serial
	Ch376msc(Stream &sUsb);// SW serial
	Ch376msc(uint8_t spiSelect, uint8_t busy);//SPI with MISO as Interrupt pin
	Ch376msc(uint8_t spiSelect, uint8_t busy, uint8_t intPin);
	virtual ~Ch376msc();//destructor
	////////////////////////////////////////////////
	void init();

	uint8_t mount();
	uint8_t dirInfoSave();
	uint8_t openFile();
	uint8_t closeFile();
	uint8_t moveCursor(uint32_t position);
	uint8_t deleteFile();
	uint8_t pingDevice();
	uint8_t listDir();
	uint8_t readFile(char* buffer, uint8_t b_num);
	uint8_t writeFile(char* buffer, uint8_t b_num);
	bool checkDrive(); // check is it any interrupt message came from CH
	//bool listDir(char (*nameBuff)[12],uint32_t *sizeBuff,uint8_t listElements); //376_7
	//void reset();

//set/get

	//uint32_t getComSpeed();
	uint32_t getFileSize();
	uint16_t getYear();
	uint16_t getMonth();
	uint16_t getDay();
	uint16_t getHour();
	uint16_t getMinute();
	uint16_t getSecond();
	uint8_t getStatus();
	char* getFileName();
	char* getFileSizeStr();
	bool getDeviceStatus(); // usb device mounted, unmounted
	bool getCHpresence();

	void setFileName(const char* filename);
	void setYear(uint16_t year);
	void setMonth(uint16_t month);
	void setDay(uint16_t day);
	void setHour(uint16_t hour);
	void setMinute(uint16_t minute);
	void setSecond(uint16_t second);

private:
	//
	//uint8_t read();
	void write(uint8_t data);
	void print(const char str[]);
	void spiReady();
	void spiBeginTransfer();
	void spiEndTransfer();
	void waitSpiInterrupt();
	uint8_t spiReadData();

	uint8_t getInterrupt();
	uint8_t fileEnumGo();
	uint8_t byteRdGo();
	uint8_t fileCreate();
	uint8_t byteWrGo();
	uint8_t reqByteRead(uint8_t a);
	uint8_t reqByteWrite(uint8_t a);
	uint8_t readSerDataUSB();
	uint8_t writeDataFromBuff(char* buffer);
	uint8_t readDataToBuff(char* buffer);
	uint8_t dirInfoRead();
	uint8_t setMode(uint8_t mode);

	void rdUsbData();
	void setSpeed();
	void sendCommand(uint8_t b_parancs);
	void sendFilename();
	void writeFatData();
	void constructDate(uint16_t value, uint8_t ymd);
	void constructTime(uint16_t value, uint8_t hms);
//#define TEST
#ifdef TEST

	void fileReOpen(); // if cluster is full
	void wrDummyByte(uint8_t asd);
#endif
	///////Global Variables///////////////////////////////
	bool _fileWrite = false; // read or write mode, needed for close operation
	bool _mediaStatus;				//false USB levalsztva, true csatlakoztatva
	bool _controllerReady = false; // ha sikeres a kommunikacio

	uint8_t _byteCounter = 0; //vital variable for proper reading,writing
	uint8_t _answer = 0;	//a CH jelenlegi statusza INTERRUPT
	uint8_t _spiChipSelect; // chip select pin SPI
	uint8_t _spiBusy; //   busy pin SPI
	uint8_t _intPin; // interrupt pin
	uint16_t _sectorCounter = 0;// variable for proper reading
	uint32_t _speed; // Serial communication speed
	char _filename[12];

	HardwareSerial* _comPortHW; // Serial interface
	Stream* _comPort;

	commInterface _interface;
	fileProcessENUM fileProcesSTM = REQUEST;

	fatFileInfo _fileData;


};//end class

#endif /* Ch376msc_H_ */


