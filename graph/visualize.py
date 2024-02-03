#%%
#%matplotlib widget
import matplotlib.pyplot as plt
import csv
#%%
x,y,z=[],[],[]
with open("lattice_data.csv",'r') as file:
    reader=csv.reader(file)
    for row in reader:
        x.append(float(row[0]))
        y.append(float(row[1]))
        z.append(float(row[2]))
#%%
fig=plt.figure()
ax=fig.add_subplot(111,projection='3d')
ax.scatter(x,y,z)
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
plt.show()

# %%

