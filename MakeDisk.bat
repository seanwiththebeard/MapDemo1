copy template.dsk hello.dsk

java -jar ac.jar -p hello.dsk DEMO.system sys < C:\Dev\cc65\target\apple2\util\loader.system
java -jar ac.jar -as hello.dsk DEMO bin < prog.apple2