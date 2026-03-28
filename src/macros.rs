

#[macro_export]
macro_rules! define_enum {
    ($name:ident, $($variant:ident),*) => {
        paste::paste! {
            pub trait [<Is $name>] {
                const VALUE: $name;
            }

            #[derive(Debug, Clone, PartialEq, Copy, Eq)]
            pub enum $name {
                $($variant),*
            }
            $(
                #[derive(Debug, Clone, PartialEq, Copy, Eq)]            
                pub struct [<$variant Type>];
                impl [<Is $name>] for [<$variant Type>] {
                    const VALUE: $name = $name::$variant;
                }
            )*
        }
    };
}