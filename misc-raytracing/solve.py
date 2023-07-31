import subprocess


def start(executable_file):
    return subprocess.Popen(
        executable_file,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE)


def read(process):
    return process.stdout.readline().decode("utf-8").strip()


def write(process, message):
    process.stdin.write(f"{message.strip()}\n".encode("utf-8"))
    process.stdin.flush()


def terminate(process):
    process.stdin.close()
    process.terminate()
    process.wait(timeout=0.2)

DEBUG_INFO = False

def cast_ray(start:tuple, end:tuple, process):
    a = read(process)
    if DEBUG_INFO:
        print(a)
    for coordinate in start:
        write(process, str(coordinate))
        a = read(process)
        if DEBUG_INFO:
            print(a)
    
    a = read(process)
    if DEBUG_INFO:
        print(a)

    for coordinate in end:
        write(process, str(coordinate))
        result = read(process)
        if DEBUG_INFO:
            print(result)
        if result == "Miss!":
            # print("X")
            return "."
        elif result == "Hit!":
            # print("Y")
            return "√"
    # print(result)
    result = read(process)
    if DEBUG_INFO:
        print(result)
    if result == "Miss!":
        # print("X")
        return "."
    elif result == "Hit!":
        # print("Y")
        return "√"
    

process = start("./rt")
print(read(process))
# print(read(process))

# write(process, "1")
# result = read(process)
# print(result)
# if result == "Miss!":
#     print("X")

# print(read(process))
# cast_ray((0,0,0),(100,100,100),process)

scan_result = []

y_range = 200
x_range = 4000

# xy
for y in range(0,y_range,10):
    for x in range(0,x_range,10):
        ret = cast_ray((x,y_range-y,0),(x,y_range-y,40),process)
        print(ret, end=" ")
        scan_result.append(ret)
        # print("casted!\n")
        # if y < 50:
        #     continue
        # exit(0)
    print()
    

y_range = 200
x_range = 4000

# xy
for y in range(0,y_range,10):
    for x in range(0,x_range,10):
        ret = cast_ray((x+30,y_range-y,0),(x,y_range-y,40),process)
        print(ret, end=" ")
        scan_result.append(ret)
        # print("casted!\n")
        # if y < 50:
        #     continue
        # exit(0)
    print()

##### xz
for y in range(0,y_range,10):
    for x in range(0,x_range,10):
        ret = cast_ray((x,0,y_range-y),(x,40,y_range-y),process)
        print(ret, end=" ")
        scan_result.append(ret)
        # print("casted!\n")
        # if y < 50:
        #     continue
        # exit(0)
    print()

##### yz
# y_range = 200
# z_range = 200    
# x_range = 1000
# x_interval = 120

# for x in range(450,x_range,x_interval):
#     for y in range(0,y_range,10):
#         for z in range(0,z_range,10):
#             ray_origin = (x,y_range-y,z)
#             ray_end = (x+x_interval,y_range-y,z)
#             # print(ray_origin, ray_end)
#             ret = cast_ray(ray_origin,ray_end,process)
#             print(ret, end=" ")
#             scan_result.append(ret)
#             # print("casted!\n")
#             # if y < 50:
#             #     continue
#             # exit(0)
#         print()


##### Oblique example
##### vary x to get all characters

# y_range = 200
# z_range = 200    
# x_range = 700
# x_interval = 120
# x_start = 650

# for x in range(x_start,x_range,x_interval):
#     for y in range(0,y_range,10):
#         for z in range(0,z_range,10):
#             ray_origin = (x,y_range-y,z)
#             ray_end = (x+x_interval,y_range-y,z-120)
#             # print(ray_origin, ray_end)
#             ret = cast_ray(ray_origin,ray_end,process)
#             print(ret, end=" ")
#             scan_result.append(ret)
#             # print("casted!\n")
#             # if y < 50:
#             #     continue
#             # exit(0)
#         print()

y_range = 200
z_range = 200    
x_range = 1620
x_interval = 120
x_start = 1510

for x in range(x_start,x_range,x_interval):
    for y in range(0,y_range,10):
        for z in range(0,z_range,10):
            ray_origin = (x,y_range-y,z)
            ray_end = (x+x_interval,y_range-y,z+x_interval)
            # print(ray_origin, ray_end)
            ret = cast_ray(ray_origin,ray_end,process)
            print(ret, end=" ")
            scan_result.append(ret)
            # print("casted!\n")
            # if y < 50:
            #     continue
            # exit(0)
        print()
        

x_start = 1760
x_range = 1880

for x in range(x_start,x_range,x_interval):
    for y in range(0,y_range,10):
        for z in range(0,z_range,10):
            ray_origin = (x,y_range-y,z)
            ray_end = (x+x_interval,y_range-y,z+x_interval)
            # print(ray_origin, ray_end)
            ret = cast_ray(ray_origin,ray_end,process)
            print(ret, end=" ")
            scan_result.append(ret)
            # print("casted!\n")
            # if y < 50:
            #     continue
            # exit(0)
        print()
           
# for x in range(0,2000,10):
#     for y in range(0,2000,10):
#         print(scan_result)

terminate(process)