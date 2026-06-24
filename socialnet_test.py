import random
with open("input.txt","w") as f:
    add = lambda n,j: f.write(f"ADD_USER {n}\n")
    addf = lambda i,j: f.write(f"ADD_FRIEND {i} {j}\n")
    listf = lambda i,j:  f.write(f"LIST_FRIENDS {i}\n")
    sugf = lambda i,j: f.write(f" SUGGEST_FRIENDS {i} {j}\n")
    dof = lambda i,j:  f.write(f"DEGREES_OF_SEPARATION {i} {j}\n")
    addp = lambda i,j: f.write(f"ADD_POST {i} hello this is {i} from {j}\n")
    outp = lambda i,j: f.write(f"OUTPUT_POSTS {i} {j}\n")
    l = [add,addf,listf,sugf,dof,addp,outp]
    numbers = random.sample(range(0, 9), 5)
    for i in range(len(numbers)):
        add(numbers[i],1)
    for _ in range(100):
        n1 = random.randint(0,6)
        n2 = random.randint(0,9)
        n3 = random.randint(0,9)
        l[n1](n2,n3)

    # for i in range(40):
    #     f.write(f"ADD_USER {i}\n")
    # for i in range(25):
    #     for j in range(0,25,2):
    #         f.write(f"ADD_FRIEND {i} {j}\n")
    # for i in range(0,55,3):
    #     f.write(f"LIST_FRIENDS {i}\n")
    # for i in range(0,38,4):
    #     for j in range(-5,60,2):
    #         f.write(f" SUGGEST_FRIENDS {i} {j}\n")
    # for i in range(36):
    #     for j in range(25):
    #         f.write(f"DEGREES_OF_SEPARATION {i} {j}\n")
    # for i in range(20):
    #     for j in range(0,21,2):
    #         f.write(f"ADD_POST {i} hello this is {i} from {j}\n")
    # for i in range(20):
    #     for j in range(-3,19,2):
    #         f.write(f"OUTPUT_POSTS {i} {j}\n")
        
