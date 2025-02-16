from openai import OpenAI
import sys
import json
from dotenv import load_dotenv
import os

load_dotenv()
client = OpenAI(
  api_key=os.getenv("OPENAI_API_KEY")
)

system_message = """
You are tasked with giving the coordinates of the positions of objects in order to form a picture.
The User will input the picture they want you to make.

You are in a 3D matrix with the coordinates (x,y,z). Each coordinate is an index of where it is in a matrix.
You are in the positive quadrant and can only use positive numbers. The origin is (0,0,0)
The dimensions of the matrix is 20x20x20
You have to form the image with the basic objects: Cube, Pyramid, Sphere, Cylinder, and Cones.

To form a full object in a matrix you can stack the primative objects on top, below or beside each other.

For example placing a cube at (0,1,0) and placing a pyramid at (0,2,0) will put the pyramid on top of the cube.

IMPORTANT:
When trying to put a pyramid on top of a cube, the cubes can only be stacked on top of eachother, NEVER beside.
When given something like the moon or the sun, there should only be ONE sphere representing it
When the prompt asks for a lot of objects, do not make all of the objects all next to eachother, spread them out reasonably.

BACKGROUND:
When asked to put something in the background, the background should be set along the x and z axis.
The indices of the x and z coordinate of any object that is a part of the background can NEVER be greater than 2
When deciding the positions of objects, set the background first.
Valid background indices:
(0,0,0),(1,3,1),(2,7,2)
Invalid background indices:
(3,0,1),(0,0,5),(7,4,1)

## **VARIANCE FOR REPEATED OBJECTS:**

### **Buildings (or any repeated vertical objects)**
- Choose a **base height `H` for the first building** based on the input (e.g., `H = 10`).
- For each additional building, **adjust the height using the formula:**
  - Pick a random number `N` between `0` and `H // 2`
  - If `N` is **odd**, subtract it from `H`  
  - If `N` is **even**, add it to `H`  
  - Ensure `H` remains within reasonable bounds (e.g., 3 ≤ H ≤ 15).

### **Trees**
- Trees should have **variable trunk heights** and **random leaf sizes**:
  - The **trunk (cylinder)** should be between **3 to 7** units tall.
  - The **leaves (sphere/cone)** should be between **1 to 3** units tall.

### **Mountains**
- Mountain peaks should **never be the same height.**
- Vary them by ±2 to **simulate a natural range**.

EXAMPLE EXPECTED OUTPUT GIVEN THE INPUT OF DRAWING A HOUSE TREES WITH A BACKGROUND OF MOUNTAINS:
  {"type": "Cube", "position": [8,0,8]},
  {"type": "Cube", "position": [8,1,8]},
  {"type": "Cube", "position": [8,2,8]},
  {"type": "Cube", "position": [8,3,8]},
  {"type": "Cube", "position": [9,0,8]},
  {"type": "Cube", "position": [9,1,8]},
  {"type": "Cube", "position": [9,2,8]},
  {"type": "Cube", "position": [9,3,8]},
  {"type": "Cube", "position": [8,0,9]},
  {"type": "Cube", "position": [8,1,9]},
  {"type": "Cube", "position": [8,2,9]},
  {"type": "Cube", "position": [8,3,9]},
  {"type": "Cube", "position": [9,0,9]},
  {"type": "Cube", "position": [9,1,9]},
  {"type": "Cube", "position": [9,2,9]},
  {"type": "Cube", "position": [9,3,9]},
  {"type": "Pyramid", "position": [8,4,8]},
  {"type": "Pyramid", "position": [9,4,8]},
  {"type": "Cylinder", "position": [5,0,5]},
  {"type": "Cylinder", "position": [5,1,5]},
  {"type": "Sphere", "position": [5,2,5]},
  {"type": "Cylinder", "position": [12,0,5]},
  {"type": "Cylinder", "position": [12,1,5]},
  {"type": "Sphere", "position": [12,2,5]},
  {"type": "Pyramid", "position": [0,0,0]},
  {"type": "Pyramid", "position": [1,0,0]},
  {"type": "Pyramid", "position": [2,0,0]},
  {"type": "Pyramid", "position": [0,0,1]},
  {"type": "Pyramid", "position": [1,0,1]},
  {"type": "Pyramid", "position": [2,0,1]},
  {"type": "Pyramid", "position": [0,0,2]},
  {"type": "Pyramid", "position": [1,0,2]},
  {"type": "Pyramid", "position": [2,0,2]}

This example provides good instruction on how to make these objects, when making these specific objects, vary the width and height.
  
EXPECTED OUTPUT FORMAT:
[
  {"type": "Cube", "position": [0,1,0]},
  {"type": "Pyramid", "position": [0,2,0]}
]
....

Do not provide any explanation or anything extra just the expected output, Follow a strict JSON format, The output has to be a valid JSON file
NEVER add anything extra to the output like ``` NEVER.

"""

#user_message = sys.argv[1]
user_message = "Please draw a landscape of a dense forest."

completion = client.chat.completions.create(
  model="gpt-4o",
  temperature=0.2,
  messages=[
    {"role": "developer", "content": system_message},
    {"role": "user", "content": user_message}
  ]
)

response = completion.choices[0].message.content

print(response)

response_json = json.loads(response)

with open("objects.json", "w") as file:
  json.dump(response_json,file,indent=2)