#![allow(non_snake_case, non_upper_case_globals)]

use dioxus::prelude::*;

const DefaultSectionClass: &'static str = "primarySection";

fn prepareClass(text: &'static str, isRight: bool, defaultValue: Option<&'static str>) -> String
{
	let mut class = "".to_string();
	if let Some(value) = defaultValue
	{
		class += value;
		class += " ";
	}
	
	if isRight
	{
		class += " right ";
	}
	
	class += text;
	
	return class;
}

#[derive(PartialEq, Props)]
pub struct SectionProps
{
	label: &'static str,
	content: &'static str,
	#[props(!optional)]
	imageUrl: Option<&'static str>,
	#[props(!optional)]
	videoUrl: Option<&'static str>,
	right: Option<bool>,
	class: Option<&'static str>,
	id: Option<&'static str>,
}

pub fn Section(cx: Scope<SectionProps>) -> Element
{
	let isRight = match cx.props.right
	{
		Some(val) => val,
		None => false,
	};
	
	let finalClass = match cx.props.class
	{
		Some(text) => prepareClass(text, isRight, Some(DefaultSectionClass)),
		None => prepareClass(DefaultSectionClass, isRight, None),
	};
	
	let finalId = match cx.props.id
	{
		Some(text) => text,
		None => "",
	};
	
	let image = match cx.props.imageUrl
	{
		Some(url) => rsx!(img { src: "{url}" }),
		None => rsx!(""),
	};
	
	let mut rowClass = "row".to_string();
	let video = match cx.props.videoUrl
	{
		Some(url) => {
			rowClass += " video";
			
			rsx!(video
			{
				autoplay: true,
				controls: true,
				r#loop: true,
				muted: true,
				playsinline: true,
				preload: true,
				src: "{url}"
			})
		},
		None => rsx!(""),
	};
	
	return cx.render(rsx!
	{
		div
		{
			class: "{finalClass}",
			id: "{finalId}",
			
			h3 { "{cx.props.label}" }
			div
			{
				class: "{rowClass}",
				div { class: "content", "{cx.props.content}" }
				image
				video
			}
		}
	});
}

pub fn Spacer(cx: Scope) -> Element
{
	return cx.render(rsx!
	{
		div { class: "spacer" }
	});
}
