# Shell

Shell program is the user interface of the database. 

It receives SQL commands from the keyboard and calls parser to parse them into statements. Then calls backend to execute different statements and print out the query result. 

### 二阶段拓展

​	二阶段中实现了两个新的shell程序：shellserver和shellclient，分别是服务器和客户端上的shell。服务器默认IP为127.0.0.1，端口为1234，可以在shellserver.cpp中修改。客户端在调用时需要提供运行参数`./client  $SERVER_IP $SERVER_PORT` ，第一个参数为服务器IP，第二个参数为服务器端口，若不提供则默认为127.0.0.1和1234。客户端调用时需保证服务器已经运行，否则客户端会报错。

## License

This project is licensed under the Apache License - see the [LICENSE](../../LICENSE) file for details

