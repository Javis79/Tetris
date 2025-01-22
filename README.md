<h1>Tetris<h1>
Tetris game using C programming language for FRDM-KL05Z kit with microcontroller Freescale Kinetis KL05Z


<h2>Features</h2>
<ul>
    <li>Classic Tetris gameplay.</li>
    <li>Rendered on a 1.3-inch OLED display using SPI communication.</li>
    <li>Physical buttons and touch slider for gameplay control.</li>
    <li>Compact and efficient code.</li>
</ul>

<h2>Requirements</h2>
<h3>Hardware Components</h3>
<ol>
    <li><strong>Microcontroller:</strong> NXP KL05Z (FRDM-KL05Z development board).</li>
    <li><strong>OLED Display:</strong> 1.3-inch OLED by Waveshare (SPI communication).</li>
    <li><strong>Matrix keyboard:</strong> Used push buttons for control.</li>
    <li><strong>Touch slider (TSI):</strong> included in development board.</li>
</ol>

<h3>Software Requirements</h3>
<ul>
    <li><strong>IDE:</strong> Keil uVision5 or any compatible C/C++ IDE for ARM Cortex-M0+.</li>
    <li><strong>Toolchain:</strong> ARM GCC or similar.</li>
    <li><strong>Git:</strong> For version control.</li>
</ul>

<h2>Setup Instructions</h2>
<h3>Clone the Repository</h3>
<pre><code>
git clone https://github.com/Javis79/Tetris.git
</code></pre>
<h3>Hardware Connections</h3>
<h4>Microcontroller to OLED Display</h4>
<table border="1">
    <tr>
        <th>OLED Pin</th>
        <th>KL05Z Pin</th>
        <th>Function</th>
    </tr>
    <tr><td>RST</td><td>PTB5</td><td>Reset</td></tr>
    <tr><td>D/C</td><td>PTB7</td><td>Data/Command</td></tr>
    <tr><td>CS</td><td>PTB6</td><td>Chip Select</td></tr>
    <tr><td>DIN (MOSI)</td><td>PTA7</td><td>SPI Data</td></tr>
    <tr><td>CLK</td><td>PTB0</td><td>SPI Clock</td></tr>
    <tr><td>GND</td><td>GND</td><td>Ground</td></tr>
    <tr><td>VCC</td><td>3.3V</td><td>Power Supply</td></tr>
</table>

<h4>Matrix Keyboard to Microcontroller</h4>
<table border="1">
    <tr>
        <th>Button (Pin)</th>
        <th>KL05Z Pin</th>
        <th>Function</th>
    </tr>
    <tr><td>(R1)</td><td>GND</td><td>Enables row of buttons</td></tr>
    <tr><td>S2 (C2)</td><td>PTA10</td><td>Select level: easy/Rotate block</td></tr>
    <tr><td>S3 (C3)</td><td>PTA11</td><td>Select level: medium/Increases falling speed</td></tr>
    <tr><td>S4 (C4)</td><td>PTA12</td><td>Select level: gard/Resets falling speed</td></tr>
</table>

<h3>Build and Flash the Project</h3>
<ol>
    <li>Open the project in your IDE.</li>
    <li>Configure the correct target device (KL05Z).</li>
    <li>Compile the project.</li>
    <li>Flash the binary to the microcontroller using an appropriate programmer/debugger.</li>
</ol>

<h3>Running the Project</h3>
<ol>
    <li>Ensure all hardware connections are secure.</li>
    <li>Power the KL05Z board.</li>
    <li>The OLED display should initialize and display the Tetris introduction.</li>
    <li>The OLED display should show option to choose level.</li>
    <li>Use the buttons to select level of the game:
        <ul>
            <li><strong>S2:</strong> Easy level: blocks fall slowly, base gain of points is 1.</li>
            <li><strong>S3:</strong> Medium level: blocks fall faster, base gain of points is 2.</li>
            <li><strong>S4:</strong> Hard level: blocks fall even faster, base gain of points is 3.</li>
        </ul>
    </li>
    <li>Use the buttons and slider to play the game:
        <ul>
            <li>S2: Rotates current block.</li>
            <li>S3: Makes blocks fall faster.</li>
            <li>S4: Resets blocks falling speed.</li>
            <li>Touch slider: Sliding to left moves block to left and sliding to right moves block to the same direction.</li>
        </ul>
    </li>
</ol>

<h2>Code Structure</h2>
<ul>
    <li><code>main.c</code>: Entry point of the application.</li>
    <li><code>game.c</code> / <code>game.h</code>: Starts the game and initializes all the elements.</li>
    <li><code>spi.c</code> / <code>spi.h</code>: SPI communication driver.</li>
    <li><code>klaw.c</code> / <code>klaw.h</code>: Matrix keyboard initialization and implementation.</li>
    <li><code>menu.c</code> / <code>menu.h</code>: Non-gameplay elements like introduction screen, level selection, scoreboard screen.</li>
    <li><code>oled.c</code> / <code>oled.h</code>: OLED display driver and rendering functions.</li>
    <li><code>gpio.c</code> / <code>gpio.h</code>: GPIO abstraction layer.</li>
    <li><code>blocks.c</code> / <code>blocks.h</code>: Tetris block definitions.</li>
    <li><code>font.c</code> / <code>font.h</code>: Font data and rendering.</li>
    <li><code>area.c</code> / <code>area.h</code>: Game area management, rotating, spawning blocks, cleaning lines, counting points.</li>
    <li><code>tsi.c</code> / <code>tsi.h</code>: Touch slider interface.</li>
</ul>

<h2>Project development/development opportunities</h2>
<ul>
  <li><code>Using keyboard to move blocks</code>: Give players option to choose between controlling with touch slider and keyboard or only keyboard</li>
  <li><code>Make more game modes</code>: Game modes like "Marathon", where falling speed increases in time or "Time attack", where players have limited time to score a specyfic number of points </li>
  <li><code>Setting a nickname</code> Give players options to choose a nickname that will represent them in the high score table</li>
</ul>


<h2>Acknowledgements</h2>
<p>Special thanks to <b>mgr. inż Sebastian Koryciak</b> for his supervision and guidance during the completion of this project for the "Microprocessor Systems 2" course.</p>
