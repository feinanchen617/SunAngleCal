🌞 SunAngleCal· 太阳角度计算器
SunAngle Explorer 是一个基于 Qt 构建的应用程序，用于计算和可视化太阳高度角与方位角，适用于拍照、天文观测、环境建模等领域。

🧭 功能特点 | Features
中文功能说明	English Description
✅ 输入日期、时间、经纬度，计算太阳高度角和方位角	Input date, time, and coordinates to calculate solar azimuth & elevation
✅ 输入太阳角度，推算出最接近的时间（支持秒级）	Estimate the best matching time based on given sun angle (to the second)
✅ 输入太阳角度与时间，反推出观测点的经纬度(目前为0.5°)	Estimate approximate coordinates based on solar angle and time

💻 使用场景 | Use Cases
🛰️ 遥感：卫星传感器几何校正辅助
🌍 教育：教学展示地球-太阳几何关系
🌇 建筑：遮阳角度评估、光照分析

🛠 技术栈 | Tech Stack
Qt 5.12.5 (Widgets)
C++11
windeployqt / Inno Setup /

🗺️ 路线图 | Roadmap
 太阳角度计算
 时间反推
 经纬度反推

📃 示例截图 | Screenshot

file:///G:/build-SunAngleCal-Desktop_Qt_5_12_5_MinGW_64_bit-Release/运行示例图.png
file:///G:/build-SunAngleCal-Desktop_Qt_5_12_5_MinGW_64_bit-Release/运行示例图.png

📖 使用说明 | Instructions
填写日期与 UTC 时间
1.输入经纬度，点击“计算太阳角”按钮，太阳高度角坐标地面为0°，天顶为90°，太阳方位角正北为0°，顺时针至正东为90°以此类推。
2.或者输入太阳高度角和方位角），点击反推时间和反推位置，推算对应时间或地理位置。


📬 联系方式 | Contact
欢迎提出建议或BUG
email:feinanchen617@163.com
