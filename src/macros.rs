

pub type EnumIdType = u8;


#[macro_export]
macro_rules! define_kinds {
    ($name:ident, $($variant:ident = $num:expr),*) => {
        paste::paste! {
            #[repr(u8)]
            #[derive(Debug, Clone, PartialEq, Copy, Eq)]
            pub enum $name {
                $($variant = $num),*
            }

            impl $name {
                /// 数字を対応するバリアントに変換する
                pub const fn from_u8<const NUM: crate::macros::EnumIdType>() -> Option<$name> {
                    Some(
                        match NUM {
                            $($num => $name::$variant,)*
                            _ => return None,
                        }
                    )
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