A custom engine, current in WIP. 

# Build instructions
## Assimp
## Libjpeg-turbo

Just fallow this commands:

```sh
cd "path\to\RootEngine\libs\thirdparty\libjpegturbo\lib"

mkdir build 

cd build

cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release -DENABLE_STATIC=TRUE -DENABLE_SHARED=FALSE -DCMAKE_INSTALL_PREFIX="path\to\RootEngine\libs\thirdparty\libjpegturbo" ..

cmake --build . --config Release --target install
```

This commands will generate lib and include folders in ```"path\to\RootEngine\libs\thirdparty\libjpegturbo"```