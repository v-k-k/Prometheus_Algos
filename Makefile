all: prepare

prepare:
	rm -rf build
	mkdir build

dependency: 
	cmake -H. -Bbuild -DCMAKE_BUILD_TYPE="Debug" --graphviz=build/graph.dot && dot -Tpng build/graph.dot -o build/graphImg.png
	
	# cd build 
	#  -DENABLE_TESTING=OFF
