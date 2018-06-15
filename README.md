# JWCKnife

写着写着代码就几千了  

### Introduction

四川大学本科教务系统（URP系统）在访问量较大的时候很容易出现500 Servlet Error，体验很不友好  
于是想做一个本地端的应用（可以最小化到后台），具有本科教务系统中的大部分功能，通过本地端缓存，在减小教务系统压力的同时方便自己.....emmmm.....支持自动选课  
[![Watch the video](https://github.com/ExplosiveBattery/JWCKnife/blob/master/README/video_screenshot.png?raw=true)](https://youtu.be/qb3fSluCN9Q)  
后来我发现右下角的图片背景色是一个个格子...这个问题在我开发的电脑上没有表现出来，这是加入到项目中图片的原因，只需要替换成正常的图片即可  

### General code structure description

<pre>
Sources部分：
- main.cpp 应用入口文件
	职责：配置Application，显示LoginWindow
- function.cpp
	职责：为其他文件提供常用函数与全局变量
	补充：g_keyBuf全局变量为Check将数据写入到自己提供了预置预留
- Check文件夹
	职责：检查硬件信息
	补充：我曾弄过一个小Demo就是将MAC地址按照m_GetKey()转换为一个int数字，然后将这个数字写入到应用程序exe的空白段（找一块00比较多的字节块或者使用main.cpp预留的位置，记下地址，通过文件二进制模式打开定位位置实现）。
		这个程序中此功能没有实现，因为这个功能主要是自己一时兴起，并不是需求功能。但是我应该之后会补上。
- Network文件夹
	职责：网络行为
	补充：EducationSystem负责与四川大学本科教务系统之间的交互（为了方便，不按照网上常见写法，继承QNetworkAccessManager），包含数据处理、存储与发送。爬虫交互部分为了加快字符串的匹配速度，我正则并没有采用最简方式。数据库存议储部分，优先考虑代码简洁，所以有时使用prepare()，有时使用字符串拼接直接执行命令。
		PageList负责多线程爬虫的任务分配
- Widgets文件夹
	职责：负责界面交互
	补充：MVD文件夹存放Delegate与Model
		MVD文件夹下的AutoTurnLineDelegate文件名字错了，实现的不是自动换行功能，但是效果不错，如果需要这个功能，需要自己设置每一行的字符上限。同时提醒，需要设置一个行数上限，不然表格容易参差不齐
		MainWindow.cpp 因为是主界面的最外层窗口，可以看到构造函数中有许多connect函数，提供了信号连接。
		AccountItem 是LoginWindow下拉窗口中，表示每一个账号的子窗口
		CardArrayWidget “方案课程”功能里面的6个方块按钮
		ProfessionChooseForm 专业选择窗口，通过点击按钮选择目标专业，主要是限制用户的输入
		Stacked开头的文件，表示点击第一栏标题对应的窗口
		StackedGradesWidget中我不得不动态创建UI界面，Qt Designer处理不了内部控件数量未知的窗口
		TitleWidget 动态创建第二栏标题
		TreeViewWidget 点击“方案课程”功能中的方块按钮，所弹出的树状形式显示数据的窗口
Forms部分：
	职责：各种widget的ui文件（Qt Designer设计）
Resources部分：
	职责：font.qrc 收纳程序中会用到的字体类型
		images.qrc 收纳程序中会用到的图片
	补充：字体通过以下代码使用：
		int fontId =QFontDatabase::addApplicationFont(":/Resources/titlewidget.ttf");
	    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
	    m_font.setFamily(fontFamilies.at(0));
</pre>

### Special Knowledge point
- QT的SQLITE数据库推荐使用SQLITECIPHER，因为是加密的
- 通过QSS定义控件不同状态下的显示情况，比如LoginWindow的“登录”按钮、窗口最小化按钮、窗口关闭按钮
- QComboBox+QListWidget 实现下拉列表，结合数据库实现登录账号记录与删除（按照登录频率排序）
- 窗口拖动的时候，要注意没准用户中途会切换到鼠标右键，所以在判断的时候不仅仅需要状态，还需要对鼠标事件的判断。
- 应用程序后台执行（托盘图标）
- MainWindow::paintEvent重绘函数中，实现第一栏标题的按钮点击切换动画，通过这个函数能够知道哪些需要借助qt代码来填充，哪些需要自己进行ps
- MainWindow::handleStackButtonClick通过sender()->objectName().at(11).digitValue()巧妙地识别了点击按钮
- StackedScheduleWidget实现了好看的课程表样式（基于QTableWidget）,并且根据内容来调整右键菜单选项
- QTimer对网络发包超时进行处理
- 在QTableView中自定义图形按钮以及表格头的全选按钮（见MVD文件夹内容的几个Delegate），比如收藏按钮、勾选按钮
- QTableWidget的翻页功能，配合数据库的limit命令实现
- 多线程正则爬虫（我觉得function.cpp中findAll函数写的很好，方便了我通过正则选择自己想要的数据内容；）  
- 窗口切换动画其实很简单，本质上就是一个平移动画
- 将QSqlTableModel通过自己编写的Proxy转换为TreeViewWidget支持的model

### Announce
虽然使用代码直接产生界面具有更好的性能，但是为了方便个人开发，界面使用Qt Designer制作的ui文件，配合QSS  
所有图片都是自己弄的，大部分需要我ps处理，心里就很mmp，从此讨厌前端...  
这应该是大学期间写过最大的一个项目了（大二下），功能“绩”数据刷新后需要重新启动应用？是的，我不太想继续写下去了，各种东西心烦意乱。
