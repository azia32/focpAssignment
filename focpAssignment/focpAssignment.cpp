#include<iostream>
#include<string>
using namespace std;
int main()
{
	int damage_level;
	float families;
	int route, flood, medical, previous_aid, num_trucks = 5, num_supply = 120;
	int used_trucks;
	int used_supply;
	int full_count = 0, standard_count = 0, emergency_count = 0, defered_count = 0;

	for (int i = 1; i <= 5;i++) {
		string request_name;
		switch (i) {
			case 1:request_name = "R1 - Riverside Village";
				break;
			case 2:request_name = "R2 - Hilltop Settlement";
				break;
			case 3 :request_name= "R3 - Lakeside Camp";
				break;
			case 4:request_name = "R4 - Market Town";
				break;
			case 5:request_name = "R5 - Bridge Colony";
				break;
		}
		cout << "\t Input Disaster Details for "<< request_name<<": " << endl;
		do {
			cout << "Damage leve1 (1-low,2-medium,3-high): ";
			cin >> damage_level;
			if (damage_level < 1 || damage_level>3) {
				cout << "Invalid Input! Enter within range(1-3):";
				cin >> damage_level;
			}
		} while (damage_level < 1 || damage_level>3);
		do {
			cout << "Families affected (10-200): ";
			cin >> families;
			if (families < 10 || families>200) {
				cout << "Invalid Input.Re-enter within range(10-200): ";
				cin >> families;
			}
		} while (families < 10 || families>200);
		do {
			cout << "Is route damaged (1-yes,0-NO):";
			cin >> route;
			if (route <0 || route >1 ) {
				cout << "Invalid Input! Please enter within range(1-yes,0-no): ";
				cin >> route;
			}
		} while (route <0 || route > 1);
		do {
			cout << "Is medical needed (1-yes,0-NO)";
			cin >> medical;
			if (medical > 1 || medical < 0) {
				cout << "Invalid Input! Enter within range(1-yes,0-no): ";
				cin >> medical;
			}
		} while (medical > 1 || medical < 0);
		do {
			cout << "Is there a flood warning (1-Yes,0-No):";
			cin >> flood;
			if (flood > 1 || flood < 0) {
				cout << "Invalid Input! Enter within range(1-yes,0-no): ";
				cin >> flood;
			}
		} while (flood > 1 || flood < 0);
		do {
			cout << "Has aid been provided previously(1-Yes,0-No): ";
			cin >> previous_aid;
			if (previous_aid > 1 || previous_aid < 0) {
				cout << "Invalid Input! Enter within range(1-yes,0-no): ";
				cin >> previous_aid;
			}
		} while (previous_aid > 1 || previous_aid < 0);

		int route_score = (route == 1) ? 15 : 0;
		int medical_score = (medical == 1) ? 12 : 0;
		int flood_score = (flood == 1) ? 30 : 0;
		int aid_score = (previous_aid == 1) ? -10 : 0;

		float score = (damage_level * 10) + (families / 5) + route_score + medical_score + flood_score + aid_score;
		cout << score << endl;
		if(num_trucks>0 && num_supply >0){
		if (score >= 90) {
			cout << "Score:" << score << endl;
			if (num_trucks >= 2 && num_supply >= 50) {
				used_trucks = 2;
				used_supply = 50;
				cout << "Treatment: Full Relief" << endl;
				full_count++;
			}
			else if (num_trucks >= 1 && num_supply >= 30) {
				used_trucks = 1;
				used_supply = 30;
				cout << "Treatment: Downgraded to Standard relief (Insufficient Supplies) " << endl;
				standard_count++;
			}
			else if (num_trucks >= 1 && num_supply <= 10) {
				used_trucks = 2;
				used_supply = 20;
				cout << "Treatment: Downgraded to Emergency Relief (Severely short on supplies)" << endl;
				emergency_count++;
			}
			else {
				used_trucks = 0;
				used_supply = 0;
				cout << "Treatment: Downgraded to Defered (Out of supplies)" << endl;
				defered_count++;
			}
		}
		else if (score >= 60) {
			cout << "Score: " << score << endl;
			if (num_trucks >= 1 && num_supply >= 30) {
				used_trucks = 1;
				used_supply = 30;
				cout << "Treatment: Standard Relief" << endl;
				standard_count++;
			}
			else if (num_trucks >= 1 && num_supply >= 10) {
				used_trucks = 1;
				used_supply = 10;
				cout << "Treatment: Downgraded to Emergency Relief(Insuffiecent supplies)" << endl;
				emergency_count++;
			}
		}
			else {
				used_trucks = 0;
				used_supply = 0;
				cout << "Treatment: Downgraded To Defered (Out Of Supplies)" << endl;
				defered_count++;
			}
		}
		else if (score >= 40) {
			cout << "Score: " << score << endl;
			if (num_trucks >= 1 && num_supply >= 10) {
				cout << "Treatment:Emergency Relief" << endl;
				used_trucks = 1;
				used_supply = 10;
				emergency_count++;
			}
			else {
				cout << "Treatment:Downgraded to Defered (Out Of Supplies)" << endl;
				used_trucks = 0;
				used_supply = 0;
				defered_count++;
			}
		}
		else {
			cout << "Score: " << score << endl;
			cout << "Treatment: Defered" << endl;
			used_trucks = 0;
			used_supply = 0;
			defered_count++;
		}
		num_trucks -= used_trucks;
		num_supply -= used_supply;
	} 
	cout << "\t Summary " << endl;
	cout << "Full Relief Provided: " << full_count << endl;
	cout << "Standard Relief Provided: " << standard_count << endl;
	cout << "Emergency Kits Provided: " << emergency_count << endl;
	cout << "Defered Cases: " << defered_count << endl;

	return 0;
}
