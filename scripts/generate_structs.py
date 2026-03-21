# c++とrustで使うenumを生成
import yaml
import sys


def write_enum(path: str, data, head: str, target: str, member: str):
    try:
        with open(path, "w") as file:
            if ".hpp" in path:
                file.writelines(f"#pragma once\n")
            file.writelines(f"\n\n{head} {data[target]} " + "{\n")
            for member in data[member]:
                file.writelines(f"    {member},\n")
            file.writelines("}" + f"{" " if len(head) == 4 else ";"}")
    except FileNotFoundError:
        print(f"this file is not found -> {path}")

def generate():
    file_path = f'{sys.argv[1]}'

    # YAMLファイルを読み込む
    with open(file_path, 'r', encoding='utf-8') as file:
        data = yaml.safe_load(file)
        write_enum(sys.argv[2], data, "enum class", "enum_name", "member_names")
        write_enum(sys.argv[3], data, "enum", "enum_name", "member_names")
        write_enum(sys.argv[4], data, "enum class", "token_struct", "token_member")
        write_enum(sys.argv[5], data, "enum", "token_kind", "token_kind_member")

if __name__ == "__main__":
    generate()