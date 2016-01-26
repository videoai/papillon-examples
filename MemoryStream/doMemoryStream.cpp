/**
 * @author Kieron Messer
 * @file doMemoryStream.cpp
 * @brief Example on how to write and read Papillon objects to a memory stream.
 */
#include <PapillonCore.h>


using namespace papillon;

template<class T>
PResult WriteObject(const T & object, PByteArray & byteArray) {

	P_LOG_INFO << "Writing Papillon object to byte array in memory: " << object.ToString();
	PMemoryStream ms;
	ms.WriteObject(object);
	byteArray = ms.GetByteArray();
	return PResult::C_OK;
}

template<class T>
PResult ReadObject(const PByteArray & byteArray, T  & object) {
	PMemoryStream ms(byteArray);
	if(ms.ReadObject(object).Failed()) {
		return PResult::Error("Failed to read object from array");
	}
	P_LOG_INFO << "Read Papillon object from byte array from memory: " << object.ToString();
	return PResult::C_OK;
}


int main(int argc, char** argv) {

	PLog::OpenConsoleLogger("console", PLog::E_LEVEL_INFO);
	PapillonSDK::Initialise(argv[0]).OrDie();

	PByteArray byteArray;

	// Save it to a byte array in memory
	PString someString = "This is some string";
	WriteObject(someString, byteArray);

	// You could now store this byte array in some database...
	// WriteToSomeDatabase(byteArray);

	// Then at a later time you can retrieve your byte data and reconstruct the object
	// ReadFromSomeDatabase(byteArray)

	// Reconstruct your Papillon object from byte array
	PString someString2;
	ReadObject(byteArray, someString2);

	// Now lets check they are the same....
	if(someString != someString2) {
		P_LOG_INFO << "Something gone wrong";
	}


	// Remember you can do this for most Papillon objects, e.g. PFrame, PImages, PDetection, PEvent e.t.c...
	PImage image(128, 64);
	WriteObject(image, byteArray);
	PImage image2;
	ReadObject(byteArray, image2);
	if(!image.IsSame(image2)) {
		P_LOG_INFO << "Something gone wrong";
	}

	return 0;
}
