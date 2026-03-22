# c++とrustで使うenumを生成
from collections import deque
import yaml
import sys


class WriteStruct:
    def __init__(self):
        self.end = lambda end: ";\n" if self._is_cpp else f"{end}\n"

    def open(self, file_path: str):
        # 書き込むファイルがcppかrustか
        self._is_cpp = True if ".hpp" in file_path else False
        self.file = open(file_path, "w")
        if self._is_cpp:
            self.file.writelines("#pragma once\n\n")
            self.file.writelines("#include <string>\n\n")

    def write(self, data, data_name: str):
        print(data_name)
        if "enum" in data_name:
            self._write_enum_code(data[data_name], data[f"{data_name}_member"])
        elif "struct" in data_name:
            self._write_struct_code(data[data_name], data[f"{data_name}_member"])
        else:
            print("サポートされていない設定", data_name)

    def _write_enum_code(self, data_name: str, data_members: list):
        self._write_header(data_name, "enum class" if self._is_cpp else "enum")

        for member in data_members:
            self.file.writelines(f"    {member},\n")
        self.file.writelines("}" + self.end(""))

    def _write_struct_code(self, data_name: str, data_members: list):
        self._write_header(data_name, "struct")

        try:
            for member in data_members:
                self.file.writelines(
                    f"    {self._write_mem_type(member[int(self._is_cpp)])}" + 
                    f" {"" if self._is_cpp else ":"} {member[int(not self._is_cpp)]}" + 
                    f"{self.end(",")}"
                )
            self.file.writelines("}" + self.end(""))
        except:
            print("yamlファイルの設定が間違えている")
            raise

    def _write_header(self, data_name: str, mode: str):
        def check_mode(mode: str):
            match mode:
                case "enum":
                    return mode
                case "enum class":
                    return mode
                case "struct":
                    return mode
                case _:
                    raise f"このトークンはサポートされていません -> `{mode}`"
        if not self._is_cpp:
            self.file.writelines("#[repr(C)]\npub ")
        self.file.writelines(f"{check_mode(mode)} {data_name} "+"{\n")

    def _write_mem_type(self, type: str) -> str:
        if "Str" in type:
            return "std::string"
        elif "usize" in type:
            return "size_t"
        else:
            return type

    def close(self):
        self.file.close()

def generate():
    print("#[start]# 共有するデータ生成")
    file_path = f'{sys.argv[1]}'
    write_file = WriteStruct()

    # YAMLファイルを読み込む
    with open(file_path, 'r', encoding='utf-8') as file:
        data = yaml.safe_load(file)
        try:
            target_list = deque([v for v in data["GenerateList"] for _ in range(2)])
        except:
            raise "生成する情報が見つかりません"

        print(target_list)
        for f in sys.argv[2:]:
            write_file.open(f)
            create_list = target_list.popleft()
            for item in create_list:
                print(f"loading => {f} data {item}")
                write_file.write(data, item)
            write_file.close()

if __name__ == "__main__":
    generate()