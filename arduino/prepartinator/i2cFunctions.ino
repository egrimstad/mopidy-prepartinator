#include<Wire.h>
#include "Gyro.h" // Holds Constants for the Gyro board such as Registers
#include <SparkFun_MMA8452Q.h>


// These variables hold the gyro values. Updates by calling gyroRead()
signed int gyroX;
signed int gyroY;
signed int gyroZ;


MMA8452Q acc; //Accelerometer object
// These variables hold the accelerometer values. Updates by calling accRead()
double accX;
double accY;
double accZ;

signed int aX;
signed int aY;
signed int aZ;

char orientationMap[] = {'u', 'd', 'l', 'r', 'b', 'f'};
const int bufLen = 10;
char buf[bufLen];
int bufLoc = 0;

void i2c_setup(){
  Wire.begin(SDA, SCL);
  //gyroSetup();
  accSetup();
  Serial.println("i2c setup successful");
}

// Updates all the values of the gyro and accelereometer
void i2c_update(){
  accUpdate();
  //gyroUpdate();
}


////////
// Accelerometer
////////
static void accSetup(){
  acc.init();
  Serial.println("Accelerometer setup successful");
}

// Updates all the accelerometer values
static void accUpdate(){
  accRead();
  accAngles();
  accOrientation();
}

// Updates accelerometer values
static void accRead(){
  if(acc.available()){
    acc.read();
    accX = acc.cx;
    accY = acc.cy;
    accZ = acc.cz;
  }
}

// Calculates angle of device based on accelerometer values 
// (This only works when the device is affected by the gravitational force only)
static void accAngles(){
  aX = 90*accX;
  aY = 90*accY;
  aZ = 90*accZ;
}

// Calculates orientation from angle values and stores the [bufLen] last readings in a buffer
static void accOrientation(){
  int orientation;
  if(aZ > 45){ // UP
    orientation = 0;
  }else if(aZ < - 45){ // DOWN
    orientation = 1;
  }else if(aX > 45){ // LEFT
    orientation = 2;
  }else if(aX < -45){ // RIGHT
    orientation = 3;
  }else if(aY > 45){ // BACK
    orientation = 4;
  }else if(aY < -45){ // FRONT
    orientation = 5;
  }else{ // Could not figure out orientation
    orientation = 6;
  }
  buf[bufLoc] = orientation;
  bufLoc++;
  bufLoc = bufLoc % bufLen;
  //Serial.println(orientation);
}

// Prints the accelerometer values
void i2c_accPrint(){
  Serial.print("Acc: ");
  Serial.print(accX);
  Serial.print("\t");
  Serial.print(accY);
  Serial.print("\t");
  Serial.println(accZ);
}

// Prints the calculates angles
void i2c_anglesPrint(){
  Serial.print("Angles: ");
  Serial.print(aX);
  Serial.print("\t");
  Serial.print(aY);
  Serial.print("\t");
  Serial.println(aZ);
}

// Returns the calculated orientation by taking the max value of the readings in the buffer (to avoid reading one bad value)
char i2c_getOrientation(){
  int amount[] = {0,0,0,0,0,0};
  for(int i = 0; i < bufLen;i++){
    amount[buf[i]]++;
  }

  // Find max
  int mi = 0;
  for(int i = 1; i < 6; i++){
    if(amount[i]>amount[mi]){
      mi = i;
    }
  }
  return orientationMap[mi];
}

////////
// Gyro
////////
static void gyroSetup(){
  i2cCheck(gyroAddress, 0x00);
  
  //Configure the gyroscope
  //Set the gyroscope scale for the outputs to +/-2000 degrees per second
  i2cWrite(gyroAddress, DLPF_FS, (DLPF_FS_SEL_0|DLPF_FS_SEL_1|DLPF_CFG_0));
  //Set the sample rate to 100 hz
  i2cWrite(gyroAddress, SMPLRT_DIV, 9);

  Serial.println("Gyro setup successful");
}

static void gyroUpdate(){
  gyroRead();
}

// Updates gyro values
static void gyroRead(){
  gyroX = (signed char) i2cRead(gyroAddress, GYRO_XOUT_L) | (signed char) i2cRead(gyroAddress, GYRO_XOUT_H) << 8;
  gyroX += gyroXoff;
  
  gyroY = (signed char) i2cRead(gyroAddress, GYRO_YOUT_L) | (signed char) i2cRead(gyroAddress, GYRO_YOUT_H) << 8;
  gyroY += gyroYoff;

  gyroZ = (signed char) i2cRead(gyroAddress, GYRO_ZOUT_L) | (signed char) i2cRead(gyroAddress, GYRO_ZOUT_H) << 8;
  gyroZ += gyroZoff;
}

// Prints the contents of an i2c register (used for checking id)
static void i2cCheck(char addr, char reg){
  char id = 0;
  id = i2cRead(addr, reg);
  Serial.print("ID: ");
  Serial.println(id, HEX);
}

// Reads a register of an i2c device
static unsigned char i2cRead(char address, char registerAddress){
  unsigned char data=0; //This variable will hold the contents read from the i2c device.

  Wire.beginTransmission(address); //Send the register address to be read.
  Wire.write(registerAddress); //Send the Register Address
  Wire.endTransmission(); //End the communication sequence.

  Wire.beginTransmission(address); //Ask the I2C device for data
  Wire.requestFrom(address, 1);
  while(!Wire.available()); //Wait for a response from the I2C device
  data = Wire.read();
  Wire.endTransmission(); //End the communication sequence.

  return data;
}

// Writes a byte to an i2c device
static void i2cWrite(char address, char registerAddress, char data)
{
  Wire.beginTransmission(address); //Initiate a communication sequence with the desired i2c device
  Wire.write(registerAddress); //Tell the I2C address which register we are writing to
  Wire.write(data); //Send the value to write to the specified register
  Wire.endTransmission(); //End the communication sequence
}
