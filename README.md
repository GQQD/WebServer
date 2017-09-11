原本这个项目只是一个http服务器<br>
当我实现CGI功能之后发现了新天地<br>
于是乎通过CGI+Python,可以实现一个在线的漏洞扫描平台<br>
因为好奇，所以努力....加油!<br>

<h1>服务器</h1>
单线程的epoll处理http请求完毕<br>
多线程的epoll处理http请求时发现一个非常奇怪的问题:两个不同的线程处理了一个sock.<br>
百思不得其解.......Why???<br>
<h2>并发测试数据</h2>
<p>自主研发httpd测试数据</p>
<p>1000 clients, running 30 sec.</p>
<p>Speed=835032 pages/min, 58118052 bytes/sec.</p>
<p>Requests: 417516 susceed, 0 failed.</p>
<p>apache2的测试数据</p>
<p>1000 clients, running 30 sec.</p>
<p>Speed=504626 pages/min, 3441571 bytes/sec.</p>
<p>Requests: 252313 susceed, 0 failed.</p>
<p>几百行的小程序居然比apache2的并发性还高!!!!</p>
<p>仔细想想也释然:毕竟apache2比较大,功能比较多....</p>

<h1>Python漏洞扫描</h1>
<p>2017-07-27 CDN检测模块完成!(耗时:20+s)</p>
<p>2017-07-28 端口扫描模块完成!(扫描52个端口,耗时5s)</p>
<p>2017-07-30 动态页面抓取爬虫完成!(根据网站规模决定,平均:5-10s)</p>
<p>2017-08-03 sql注入漏洞检测模块完成!(耗时:1s以内)</p>
<p>2017-08-11 cms指纹检测模块完成!(耗时:50+s)</p>
<p>2017-08-12 webshell爆破模块完成!(耗时:1s)</p>
<p>2017-08-13 XSS跨站脚本漏洞扫描模块完成!(耗时:1s)</p>
<p>2017-08-16 网站备份检测模块完成!(耗时:3s)</p>
<p>等待整合...</p>
