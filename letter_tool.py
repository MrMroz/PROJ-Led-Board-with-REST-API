import cv2
import os
import np


OUT_DIR = os.path.join(os.getcwd(), "output")
IN_DIR = os.path.join(os.getcwd(), "input")

def in_pth(img_name: str) -> str:
    return os.path.join(IN_DIR, img_name)

img = cv2.imread(in_pth('A.png'), -1)
img.astype(np.uint8)
# img = cv2.cvtColor(img, cv2.COLOR_B)

#img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
# with open('A.txt', mode='w') as f:
#     for s in img:
#         f.write(str(s))


print(len(img))
print(len(img[0]))
print(len(img[0][0]))

TEMPLATE = "uint8_t A[19][64] = {"

out = TEMPLATE

for row in img:
    out += '\n  '
    for pixel in row:
        if pixel[3] == 255:
            out += '0xFF, '
        else:
            out += '0x00, '
        

out = out[:-2]

out += '}'

with open('A.txt', mode='w', encoding='utf-8') as f:
    f.write(out)