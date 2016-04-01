INCLUDE_DIRECTORIES("${PROJECT_INCLUDE_DIR}")

SET(COMMON_SRCS 
	MundoServidor.cpp 
	ListaDisparos.cpp
	Esfera.cpp
	Plano.cpp
	Raqueta.cpp
	Vector2D.cpp
	Socket.cpp)
SET(COMMON_SRCS2 
	MundoCliente.cpp 
	ListaDisparos.cpp
	Esfera.cpp
	Plano.cpp
	Raqueta.cpp
	Socket.cpp
	Vector2D.cpp)

find_package (Threads)
				
ADD_EXECUTABLE(servidor servidor.cpp ${COMMON_SRCS})
ADD_EXECUTABLE(cliente cliente.cpp ${COMMON_SRCS2})
ADD_EXECUTABLE(logger logger.cpp)
ADD_EXECUTABLE(bot bot.cpp)

TARGET_LINK_LIBRARIES(servidor glut GL GLU)
TARGET_LINK_LIBRARIES(cliente glut GL GLU)
target_link_libraries (servidor ${CMAKE_THREAD_LIBS_INIT})


