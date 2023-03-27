#![allow(dead_code, non_snake_case, non_upper_case_globals)]

use dioxus::prelude::*;
use crate::components::section::{Section, Spacer};
use crate::components::bg::VideoBackground;

const blurb1: &'static str = r#"
This is a dice roller application that actually ROLLS the dice!
Using the power of Godot 4 and its built-in physics engine the
dice will bounce and roll around just like real dice. The values
are determined by detecting which face is pointing up for each
die when they come to a complete stop based on their position
and rotation in 3D space. It will even find the closest match
for dice that get stuck on an edge or otherwise aren't sitting
flat on the tray!
"#;

const blurb2: &'static str = r#"
Leveraging the power of Godot 4's GDExtension system, which
provides the capability to write scripts in virtually any language,
this project's code is written entirely in C++ using the official
godot-cpp bindings for the GDExtensions API. It's also set up to
build using CMake and Ninja. You can take a look under the hood
and see how everything works over at the Github repository.
"#;

const blurb3: &'static str = r#"
Dice Roller (Godot) is licensed under the MIT License so you are
free to use, modify, and redistribute it. This project was originally
created to be a portfolio piece demonstrating my abilities as a
Full Stack Software Developer. But maybe you need to roll for your
custom dragon's breath weapon damage and you don't have 25d10 on hand.
Or you want to see a real world example of how to use godot-cpp and
create a GDExtension with C++ and CMake. Or you're curious about how
the layout of this very site was implemented. It's all available for
you in the Github repository!
"#;

#[derive(Default)]
struct SectionData
{
	pub content: &'static str,
	pub imageUrl: Option<&'static str>,
	pub videoUrl: Option<&'static str>,
	pub label: &'static str,
	pub right: bool,
}

impl SectionData
{
	fn text(label: &'static str, content: &'static str, right: bool) -> Self
	{
		SectionData { content, label, right, ..Default::default() }
	}
	
	fn image(label: &'static str, content: &'static str, imageUrl: &'static str, right: bool) -> Self
	{
		SectionData { content, imageUrl: Some(imageUrl), label, right, ..Default::default() }
	}
	
	fn video(label: &'static str, content: &'static str, right: bool, videoUrl: &'static str) -> Self
	{
		SectionData { content, label, right, videoUrl: Some(videoUrl), ..Default::default() }
	}
}

pub fn App(cx: Scope) -> Element
{
	let sections = vec![
		SectionData::video("Roll The Dice", blurb1, false, "./assets/roller/roll-example.webm"),
		SectionData::text("Powered By C++", blurb2, true),
		SectionData::text("Open Source", blurb3, false),
	];
	
	return cx.render(rsx!
	{
		div
		{
			class: "app",
				
			VideoBackground
			{
				class: "parallax",
				poster: "assets/roller/page-background-still.png",
				source: "assets/roller/page-background-1.webm",
			}
			
			header
			{
				h1 { "Dice Roller (Godot)" }
				a
				{
					class: "github",
					href: "https://github.com/nemesisx00/dice-roller-godot",
					title: "https://github.com/nemesisx00/dice-roller-godot",
					img { alt: "Github", src: "assets/github-mark/github-mark-white.svg" }
				}
			}
			
			div { class: "transparentColumn" }
			
			div
			{
				class: "mainContent",
				
				Spacer {}
				
				for section in sections.iter()
				{
					Section
					{
						content: section.content,
						label: section.label,
						imageUrl: section.imageUrl,
						videoUrl: section.videoUrl,
						right: section.right,
					}
					
					Spacer {}
				}
			}
			
			footer
			{
				div { class: "footerText", "© 2023 Peter Lunneberg" }
				
				a
				{
					class: "github",
					target: "_blank",
					href: "https://github.com/nemesisx00",
					title: "https://github.com/nemesisx00",
					img { alt: "Github", src: "assets/github-mark/github-mark-white.svg" }
				}
				
				div { class: "verticalBorder", }
				div { class: "footerText", "Third-Party Software:" }
				
				a
				{
					class: "dioxus",
					target: "_blank",
					title: "https://dioxuslabs.com/",
					href: "https://dioxuslabs.com/",
					img { alt: "Dioxus", src: "assets/dioxus-logo.png" }
				}
				
				a
				{
					class: "godot",
					target: "_blank",
					title: "https://godotengine.org/",
					href: "https://godotengine.org/",
					img { alt: "Godot Engine", src: "assets/godot/godot-logo-edited.png" }
				}
			}
		}
	});
}
