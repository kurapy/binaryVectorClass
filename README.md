# binaryVectorClass
C++ binary vector class

オブジェクトファイルとして利用する場合には，
gcc -std=c++11  -c (ディレクトリ)/src/bvecClass.cpp -o (ディレクトリ)/bvecClass.o
として"bvecClass.o"を作成.
"bvec.hpp"をインクルードし,コンパイル時にオブジェクトファイルを一緒に読み込むことでbvecクラスが使用可能.

静的ライブラリとしてリンクできる状態にするためには,arコマンドを利用する.