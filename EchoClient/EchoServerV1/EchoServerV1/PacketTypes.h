#pragma once

struct Packet {
	Packet(short iD, float xVal, float yVal){
		id=iD; x=xVal; y=yVal;
	}
	short id;
	float x, y;
};

//union Packet {
//	struct Packet1{
//		Packet1(short iD, float xVal, float yVal){
//			id=iD; x=xVal; y=yVal;
//		}
//		short id;
//		float x, y;
//	};
//	char* info[52];
//};