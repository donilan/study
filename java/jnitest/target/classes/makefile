ROOT_PATH = ../../..
JAVA_PATH = ${ROOT_PATH}/src/main/java
PACKAGE_PATH = ${JAVA_PATH}/com/createw/jnitest
CLASS_PATH = ${ROOT_PATH}/target/classes
CLASSES = ${CLASS_PATH}/com/createw/jnitest/UserOpt.class
JAVAS = ${PACKAGE_PATH}/User.java ${PACKAGE_PATH}/UserOpt.java



libUserOpt.so: com_createw_jnitest_UserOpt.cpp com_createw_jnitest_UserOpt.h
	gcc -shared -I ${JAVA_HOME}/include com_createw_jnitest_UserOpt.cpp -o libUserOpt.so

com_createw_jnitest_UserOpt.h: ${CLASSES}
	javah -classpath ${CLASS_PATH} com.createw.jnitest.UserOpt

${CLASSES}: ${JAVAS}
	mvn -f ${ROOT_PATH}/pom.xml compile

install: libUserOpt.so
	cp libUserOpt.so ${ROOT_PATH}/src/main/resources/

clean:
	rm *~ *.so -rf