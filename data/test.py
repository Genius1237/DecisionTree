import pandas as pd

df = pd.read_csv('adult_data', header=None)
for i in df[df.columns[0]]:
	if i == '?':
		print("age has ?")
for i in df[df.columns[2]]:
	if i == '?':
		print("fnwlgt has ?")
for i in df[df.columns[4]]:
	if i == '?':
		print("educatino-num has ?")
for i in df[df.columns[10]]:
	if i == '?':
		print("capital-gain has ?")
for i in df[df.columns[11]]:
	if i == '?':
		print("capital-loss has ?")
for i in df[df.columns[12]]:
	if i == '?':
		print("hours-per-week has ?")