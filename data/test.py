import pandas as pd

df = pd.read_csv('adult_test', header=None)
to_drop = []

for i in range(len(df.index)):
	for j in range(len(df.columns)):
		if df.iloc[i, j] == "?":
			to_drop.append(i)
			break

df.drop(df.index[to_drop], inplace=True)
df.to_csv('adult_test_no_unknown', index=False, header=None)
# will have newline at the end of the file. manually remove