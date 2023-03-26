#![allow(non_snake_case,non_upper_case_globals)]

mod components;

use crate::components::App;

fn main()
{
	dioxus_web::launch(App);
}
