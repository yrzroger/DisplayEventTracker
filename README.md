# DisplayEventTracker

# 作用
用于测试显示器事件(Display events)
1. vsync event
2. hotplug event
3. mode change event

# 使用方法
1. 放到android源码目录下，执行mm编译，得到可执行档DisplayEventTracker，adb push到测试板的/system/bin/下
2. 在console下执行DisplayEventTracker -h,可以查看简单的命令说明
     usage:  DisplayEventTracker [options] 
          ------------------------------------ options --------------------------------------------- 
          [-i] vsync rate(default:1)
          [-h] help
          input 'r' to call requestNextVsync or input 'q' to exit when running
          ------------------------------------------------------------------------------------------
3. 示例命令
   DisplayEventTracker ：默认vsync rate = 1, 每个vsync到来时都会收到该event
   DisplayEventTracker -i 3 : 表示vsync rate = 3, 每3个vsync才会通知一次
   DisplayEventTracker -i 0 : 除非调用requestNextVsync，否则不会收到vsync event
