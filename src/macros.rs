

pub type EnumIdType = u8;


#[macro_export]
macro_rules! define_kinds {
    ($name:ident, $($variant:ident),*) => {
        #[repr(u8)]
        #[derive(Debug, Clone, PartialEq, Copy, Eq)]
        pub enum $name {
            $($variant),* // 0から順に自動で割り当てられる
        }

        impl $name {
            pub fn from_u8<const N: u8>() -> Option<Self> {
                // バリアントを配列に並べて、インデックスで比較する
                let variants = [$(Self::$variant),*];
                if (N as usize) < variants.len() {
                    Some(variants[N as usize])
                } else {
                    None
                }
            }
        }
    };
}

/// 列挙型のidを取得する
#[macro_export]
macro_rules! id{
    ($variant:path) => {
        $variant as EnumIdType
    };
}