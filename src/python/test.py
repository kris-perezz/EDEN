import json

# Create an example JSON file
x = {
  "name": "John",
  "age": 30,
  "city": "New York"
}

json_obj = json.dumps(x)

with open("data.json", "w") as json_file:
    json_file.write(json_obj)
