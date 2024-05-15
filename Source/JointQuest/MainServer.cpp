// Fill out your copyright notice in the Description page of Project Settings.


#include "MainServer.h"

FString AMainServer::BytestreamToString(TArray<uint8> bytes) {
	uint8* arr = new uint8[bytes.Num()];
	for (int i = 0; i < bytes.Num(); i++) {
		arr[i] = bytes[i];
	}
	return BytesToString(arr, bytes.Num());
}
/*
FJsonObject* AMainServer::StringToJson(FString str) {
	return new FJsonObject();
}
*/