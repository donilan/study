run: Hello.class libhello.so
	java -Djava.library.path=. Hello

Hello.class: Hello.java
	javac Hello.java

Hello.h: Hello.class
	javah Hello

libhello.so: Hello.h
	gcc -I${JAVA_HOME}/include -I. -shared -o libhello.so Hello.c

clean:
	rm -rf *~ *.class *.so