# CaptoGloveAPI 


CaptoGloveAPI is cpp wrapper for methods for using Capto Gloves with the given SDK. 


# Dependencies 

Depends on `pthread` library which must be used and gcc 7.5.0

### How to install gcc 7.5.0?

```
sudo apt-get install -y software-properties-common
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt update
sudo apt install g++-7 -y
```

Add symbolic pointers to new version: 

```
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 60 \
                         --slave /usr/bin/g++ g++ /usr/bin/g++-7 
sudo update-alternatives --config gcc
gcc --version
g++ --version
```

## Relevant code 

There is [LE scanner](https://code.qt.io/cgit/qt/qtconnectivity.git/tree/examples/bluetooth/lowenergyscanner?h=5.15) used as example. 
And there is application where we have continuous exchange of data between BLE server and client on 
following [link](https://code.qt.io/cgit/qt/qtconnectivity.git/tree/examples/bluetooth/heartrate-game) 


## TODO: 
- [ ] Initialize controller 
- [ ] Establish connection with BLE device
- [ ] Scan services 
- [ ] Scan characteristics 
- [ ] Check for updates of certain characteristsics/services
- [ ] Add Logger as Singleton 
- [ ] Add specific methods for reading needed characteristic 
