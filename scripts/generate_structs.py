# c++とrustで使うenumを生成
import yaml
import sys


class Write:
    def __init__(self):
        self.end = lambda end: ";\n" if self._is_cpp else f"{end}\n"

    def open(self, file_path: str):
        # 書き込むファイルがcppかrustか
        self._is_cpp = True if ".hpp" in file_path else False
        self.file = open(file_path, "w")

    def write(self, members: list, name: str, target: str):
        if target == "e":
            self._write_enum_code(name, members)
        elif target == "s":
            self._write_struct_code(name, members)
        else:
            print("このモードはサポートされていません" + target)
            raise TypeError

    def _write_enum_code(self, name: str, members: list):
        self._write_header(name, "enum class" if self._is_cpp else "enum")

        for member in members:
            self.file.writelines(f"    {member},\n")
        self.file.writelines("}" + self.end(""))

    def _write_struct_code(self, name: str, members: list):
        self._write_header(name, "struct")

        try:
            for member in members:
                self.file.writelines(
                    f"    {self._write_mem_type(member[int(self._is_cpp)])}" + 
                    f" {"" if self._is_cpp else ":"} {member[int(not self._is_cpp)]}" + 
                    f"{self.end(",")}\n"
                )
            self.file.writelines("}" + self.end(""))
        except:
            print("yamlファイルの設定が間違えている")
            raise

    def _write_header(self, name: str, mode: str):
        if not self._is_cpp:
            self.file.writelines("#[repr(C)]\n")
        self.file.writelines(f"{mode} {name} "+"{\n")

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
    file_path = f'{sys.argv[1]}'
    write_file = Write()
    LIST = [["ast_kind", "e"], ["ast", "s"], ["token_kind", "e"], ["token_struct", "s"]]

    # YAMLファイルを読み込む
    with open(file_path, 'r', encoding='utf-8') as file:
        data = yaml.safe_load(file)
        for i in range(0, 4):
            write_file.open(sys.argv[i + 2])
            if 1 < i:
                for c in [2, 3]:
                    print(i, LIST[c])
                    write_file.write(
                        data[f"{LIST[c][0]}_member"], 
                        data[LIST[c][0]],
                        LIST[c][1]
                    )
            else:
                write_file.write(
                    data[f"{LIST[i][0]}_member"], 
                    data[LIST[i][0]],
                    LIST[i][1]
                )
            write_file.close()

if __name__ == "__main__":
    generate()