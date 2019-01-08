# Osmos Lite
Personal and hobbyst remake of the multi-platform game
[Osmos](https://play.google.com/store/apps/details?id=com.hemispheregames.osmosdemo).

A repository just for educational purposes and not aimed at redistribution. All
rights to the name *Osmos* are henceforth reserved to their respective owners.

## Installing
If under a console and in the path containing this repository, type:
```
mkdir build
cd build/
cmake ..
make osmos
./osmos
```

The graphical launcher should then start up.

## Testing
To ensure the functioning of the test suite (developed under
[Catch 2](https://github.com/catchorg/Catch2)) simply have:
```
mkdir build
cd build/
cmake ..
make utest
./test/utest
```

If some alarming error fires up, you are very welcome to report it in the
Issues/Pull Requests section ;-).
