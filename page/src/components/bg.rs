#![allow(non_snake_case, non_upper_case_globals)]

use dioxus::prelude::*;

#[inline_props]
pub fn VideoBackground(
	cx: Scope, poster: &'static str, source: &'static str,
	class: Option<&'static str>, id: Option<&'static str>
) -> Element
{
	let mut finalClass = "";
	if let Some(text) = *class
	{
		finalClass = text;
	}
	
	let mut finalId = "";
	if let Some(text) = *id
	{
		finalId = text;
	}
	
	return cx.render(rsx!
	{
		video
		{
			class: "{finalClass}",
			id: "{finalId}",
			
			autoplay: true,
			controls: false,
			r#loop: true,
			muted: true,
			playsinline: true,
			preload: true,
			poster: "{poster}",
			src: "{source}"
		}
	});
}
