# EDEN
Environment Design Engine via Natural language
Hackathon Project

![image](https://github.com/user-attachments/assets/40e36818-756e-4e54-a970-480554f7b415)


## Requirements
**MUST HAVES**
- A Linux OS, (Mac Silcone can work but requires messing around and is not officially supported by this guide)
- VSCode with CMakeTools extension
  - NeoVim with [cmake-tools.nvim](https://github.com/Civitasv/cmake-tools.nvim) also works (procedure is basically the same)
 - **OpenAI API Key (GPT-4o)**
   - This API key **MUST** be in a .env in the main EDEN directory with the following format
     ```
     OPENAI_API_KEY=<your-api-key-here>
     ```

3D Renderer Requirements <br>
Install these through your system's package managers (Brew, pacman, etc.)
- GCC / Clang
- CMake
- OpenGL (Version 3.3 min.)
- GLM
- GLSL

Python Script Requirements <br>
Make sure to pip install these packages and make sure they are available in your PATH 
- openai
- python-dotenv


---

## Running EDEN
**This read me will assume you are on VSCode in order to run EDEN**
- Please make sure CMakeTools extension is installed 

**1. Get the files**
```
git clone https://github.com/kris-perezz/EDEN.git 
```

_or_

```
git clone git@github.com:kris-perezz/EDEN.git
```
**2. Open folder in VSCode** <br>
**3. Select a CMake Kit (if not done already)**
- clang or GCC should work just fine

**4. Run CMake: Build in the Command Palette** <br>
- This _should_ create the EDEN binary in out/Debug/ for you to run if all things go well

**5. 💰 Profit 💰**
- You can now use EDEN! Run the file, **out/Debug/EDEN** and enjoy!

---

## Potential Issues

_CMake can't build_
- Make sure the all the install requirements are properly installed
- Depending on how CMake is configured, sometimes the directories CMake needs to <br>
  properly link the project are not recognized properly. This is an issue depending on what <br>
  OS you are using. The solution is to manually check each #include statement to see if CMake can "see" it.

_Creating a scene does not work in the GUI_
- This might be caused by an invalid OpenAI key. Make sure your Open AI key is paid for and you still have tokens for use.
- Also check that the model you use is GPT-4o
- This could also be a .env issue. Please make sure your API key is stored like this 
  ```
  OPENAI_API_KEY=<your-api-key-here>
  ```
  inside your .env folder which **should** be in the main directory of EDEN.


---
## Credits
This project was created for in 48 hours for the HackED 2025 competition by: <br>
**Team: Basement Dwellers**
- [kris-perezz](https://github.com/kris-perezz)
- [sundance636](https://github.com/Sundance636)
- [kennek4](https://github.com/kennek4)
- [KyleNicholls19](https://github.com/KyleNicholls19)
