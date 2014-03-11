#!/usr/bin/env python2
from PIL import Image
import sys

for imgpath in sys.argv[1:]:  # prend le 1er arguments comme fichier
	img = Image.open(imgpath) #charge l'image
	img.load()
	alpha = img.split()[-1] # selectionne le canal alpha (transparance)
	alphadata = alpha.getdata() # retourne un tableau d'entier entre 0 et 0xFF (l'alpha de chaque pixels)
	alphadata = [pix if pix == 0 else 0xFF for pix in alphadata] # comprehension list : supprime l'alpha si il n'est pas entierement transparent
	alpha.putdata(alphadata)
	img.putalpha(alpha)

	with open(imgpath+'binaryalpha.png', "wb") as out: # ouverture du fichier de sortie
		img.save(out) # sauvegarde