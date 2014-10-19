

int menu_index=0;
int sub_menu_index = -1; //-1=no sub menu selected


//do everything in flat arrays to avoid nasty pointer actions
char* menu_entries[]={"Shoot", "Health", "Stats","God mode"};

void setup() {
	//attach isr_toggle
}

function isr_toggle() {
	if(sub_menu_index == -1) {
		//no sub selected -> move menu index

		menu_index += up/down-value;
		
		//TODO min max handle
	} else {
		//submenu
		sub_menu_index += up/down-value;
		//TODO min max handle
	}
}

void loop(){

	//draw
	if(sub_menu_index == -1) {
		//no sub selected -> move menu index

		draw_main_menu(menu_index)

	} else {
		draw_sub_menu(menu_index, current_sub_index){
	}

	//read button
	if(button is pushed){
		if(sub_menu_index == -1) {
			//no sub selected -> select one now
			sub_menu_index = 0;
		} else {
			if(sub_menu_index == 0){
				sub_menu_index = -1
			} else {
				//do stuff
			}
		}
		
	}
	//sleep 5ms
	delay(5);
}

void draw_main_menu(current_index){
	int begin = menu_index - 1; //always show at least one on top
	int i,end;
	if(begin < 0) {
		begin = 0;
	}
	end = begin + 3;
	for(i = begin; i < end; i++) {
		menu_string += menu_entries[menu_items[i].menu_entry] + "\n";
	}
	write_on_screen(menu_string);
}

void draw_sub_menu(main_menu, current_sub_index){
	write_on_screen(string);
}

void write_on_screen(string){
	uView.clear(PAGE);
	uView.setCursor(0,0);
	uView.print(string);
	uView.display();
}
