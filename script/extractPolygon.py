import json
import yaml
import sys
import re


for files in sys.argv[1:]:
	with open(files) as f:
		data = f.read()
		j = json.loads(data)
		bodies = j["rigidBodies"]
		for body in bodies:
			name = re.findall("\d+", body["name"])[0]
			polygons = body["polygons"]
			with open("%s.yml" % name, "w") as ymlfile:
				yaml.safe_dump({'polygons': polygons}, ymlfile)


