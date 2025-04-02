# news-server-test-java

This directory contains the source code for the testing and
example clients in the EDAF50 project.

How to compile the TestServer .jar executable (C++)
Create a bin folder, and compile classes to it
bin and src should be at the same directory level
mkdir -p bin
javac -d bin src/common/*.java src/testServer1/*.java

Create a manifest file, manifest.txt, with the content:
Main-Class: testServer1.TestServer1

Create .jar file, using the classes in bin/
jar cfm TestServer1.jar manifest.txt -C bin .

Run the .jar file:
java -jar TestServer1.jar [-nolog] <server> <port>

server - will be localhost
OBS: Must have a Server open at the port for the TestClient to work!