# c++とrustで使うenumを生成
import yaml
import sys


CPP_HEAEDER = "#pragma once\n\n"
RUST_C = "\n\n#[repr(C)]\n"

def write_enum(path: str, data, head: str, target: str, member: str):
    try:
        with open(path, "w") as file:
            if ".hpp" in path:
                file.writelines(CPP_HEAEDER)
            else:
                file.writelines(RUST_C)
            file.writelines(f"{head} {data[target]} " + "{\n")
            for member in data[member]:
                file.writelines(f"    {member},\n")
            file.writelines("}" + f"{" " if len(head) == 4 else ";"}")
    except FileNotFoundError:
        print(f"this file is not found -> {path}")

def write_struct(
        path: str, 
        data, 
        head: str, 
        target: list[str, str], 
        member_name: list[str, str]):
    try:
        with open(path, "w") as file:
            if ".hpp" in path:
                file.writelines(CPP_HEAEDER)
                file.writelines("#include <string>\n\n")
            else:
                file.writelines(RUST_C)
            file.writelines(f"{head} {data[target[1]]} " + "{\n")
            for member in data[member_name[1]]:
                file.writelines(f"    {member},\n")
            else:
                file.writelines("}" + f"{" " if len(head) == 4 else ";"}\n\n")
            if ".rs" in path:
                file.writelines(RUST_C)
            file.writelines(f"struct {data[target[0]]} " + "{\n")
            for member in data[member_name[0]]:
                if ".hpp" in path:
                    file.writelines(f"    {"std::string" if "Str" in member[1] else member[1]} {member[0]};\n")
                else:
                    file.writelines(f"    {member[0]} {member[1]},\n")
            file.writelines("}" + f"{" " if len(head) == 4 else ";"}")
    except FileNotFoundError:
        print(f"this file is not found -> {path}")

def generate():
    file_path = f'{sys.argv[1]}'
    CPP = "enum class"
    RUST = "enum"
    NAME = "enum_name"
    M = "member_names"

    # YAMLファイルを読み込む
    with open(file_path, 'r', encoding='utf-8') as file:
        data = yaml.safe_load(file)
        write_enum(sys.argv[2], data, CPP, NAME, M)
        write_enum(sys.argv[3], data, RUST, NAME, M)
        write_struct(sys.argv[4], data, CPP, ["token_struct", "token_kind"], ["token_member", "token_kind_member"])
        write_struct(sys.argv[5], data, RUST, ["token_struct", "token_kind"], ["token_member", "token_kind_member"])

if __name__ == "__main__":
    generate()