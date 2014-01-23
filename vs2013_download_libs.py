import urllib
from zipfile import ZipFile

urllib.urlretrieve("https://bitbucket.org/maxisoft/racing/downloads/VS-2013-SFML-2.1.zip", "VS-2013-SFML-2.1.zip")
urllib.urlretrieve("https://bitbucket.org/maxisoft/racing/downloads/Box2D.zip", "Box2D.zip")
urllib.urlretrieve("https://bitbucket.org/maxisoft/racing/downloads/Yaml-cpp.zip", "Yaml-cpp.zip")
urllib.urlretrieve("https://dl.dropboxusercontent.com/s/3yt31wj9vsnzzes/Boost.zip?dl=1", "Boost.zip")

with ZipFile('VS-2013-SFML-2.1.zip') as myzip:
	myzip.extractall()

with ZipFile('Box2D.zip') as myzip:
	myzip.extractall()

with ZipFile('Yaml-cpp.zip') as myzip:
	myzip.extractall()

with ZipFile('Boost.zip') as myzip:
	myzip.extractall()
