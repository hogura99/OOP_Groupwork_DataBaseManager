##一些逻辑设计上的问题

·   所有的list都换成vector
·   最小特权原则必须遵守
·   Base的名字要换一换
·   Data和Attribute的关系有点微妙
	· Base换成element
	· data换成AttributeValue<>
·   注释

##一些代码上的问题

·YC

·XQ
	· list可以用push_back，不用insert到end()
	· **not null 是说哪些参数是not null**
	· insert处，调用的时候应该是对Base指针操作
·HGR


##可能要加的东西

· 离线的文件操作，保证数据库可以脱机工作
	· 将所有的命令都记录下来
· 错误流，将错误信息输出到这里
· UPDATE SET支持多属性修改
· 