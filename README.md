A custom engine, current in WIP. 

# Build instructions
## Assimp
Just copy and paste this commands one by each:

```sh
cd "path\to\RootEngine\libs\thirdparty\assimp"

mkdir build 

cd build

cmake .. -DBUILD_SHARED_LIBS=OFF -DASSIMP_BUILD_ZLIB=ON

cmake --build . --config Release

cmake --build .
```

This commands will generate lib folder in ```"path\to\RootEngine\libs\thirdparty\assimp\build"```
And include folder in ```"path\to\RootEngine\libs\thirdparty\assimp\build"``` And ```"path\to\RootEngine\libs\thirdparty\assimp"```



## Libjpeg-turbo

Just copy and paste this commands one by each:

```sh
cd "path\to\RootEngine\libs\thirdparty\libjpegturbo"

mkdir build 

cd build

cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release -DENABLE_STATIC=TRUE -DENABLE_SHARED=FALSE -DCMAKE_INSTALL_PREFIX="path\to\RootEngine\libs\thirdparty\libjpegturbo" ..

cmake --build . --config Release --target install
```

This commands will generate lib and include folders in ```"path\to\RootEngine\libs\thirdparty\libjpegturbo"```



## LibPNG

Just copy and paste this commands one by each:

```sh
cd "path\to\RootEngine\libs\thirdparty\libpng"

mkdir build 

cd build

cmake .. -DPNG_STATIC=ON -DCMAKE_BUILD_TYPE=Release -DZLIB_INCLUDE_DIR="C:\Users\alan.bittencourt\Documents\Projects\Personal\RootEngine\libs\thirdparty\assimp\contrib\zlib;C:\Users\alan.bittencourt\Documents\Projects\Personal\RootEngine\libs\thirdparty\assimp\build\contrib\zlib" -DZLIB_LIBRARY=C:\Users\alan.bittencourt\Documents\Projects\Personal\RootEngine\libs\thirdparty\assimp\build\contrib\zlib\Release\zlibstatic.lib -G "Visual Studio 17 2022" -A x64

cmake --build . --config Release

cmake --build .
```