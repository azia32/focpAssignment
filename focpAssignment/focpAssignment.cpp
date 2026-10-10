// ============================================================================
// Relief Supply Distribution System
// ----------------------------------------------------------------------------
// Processes 5 relief requests after a flood. For each request it:
//   1. Reads the disaster details from the user
//   2. Calculates a NEED SCORE
//   3. Chooses an action (Full / Standard / Emergency / Defer)
//   4. Deducts the used trucks and supply packs from the remaining stock
//
// NEED SCORE FORMULA (weights and reasons)
//   Damage score   = damage level x 10  -> 10 / 20 / 30. Damage is the main
//                    indicator of how bad the situation is.
//   Family score   = families / 5       -> 2 to 40. More families = more people
//                    in need, but capped by the 10-200 range so it can't
//                    overpower the other factors.
//   Route damaged  = +15                -> a cut-off area is harder to reach
//                    later, so help should go early.
//   Medical need   = +12                -> lives may be at risk.
//   Flood warning  = +30                -> highest weight: more flooding is
//                    coming, so delay makes things much worse.
//   Previous aid   = -10                -> an area that already received help
//                    is less urgent than one that got nothing.
//
// DECISION RULES
//   Score >= 90 : Full Relief (2 trucks, 50 packs)
//                 -> else Standard -> else Emergency -> else Defer
//   Score 60-89 : Standard Relief (1 truck, 30 packs)
//                 -> else Emergency -> else Defer
//   Score 40-59 : Emergency Kit (1 truck, 10 packs)
//                 -> else Defer
//   Score < 40  : Defer (0 trucks, 0 packs)
//   "else" means: if resources are too low for the action, fall back to the
//   next smaller one. This way we never use more than what is available.
// ============================================================================

#include <iostream>
#include <string>
#include <iomanip>   // for setw, setprecision, fixed, left
using namespace std;

int main()
{
	// ---------- Resources (change these to test sufficient/insufficient stock) ----------
	int num_trucks = 5;
	int num_supply = 120;

	// ---------- Per-request input variables ----------
	int damage_level;      // 1 = low, 2 = medium, 3 = high
	int families;          // 10 - 200
	int route;             // 1 = route damaged, 0 = route open
	int medical;           // 1 = medical need, 0 = no
	int flood;             // 1 = flood warning, 0 = no
	int previous_aid;      // 1 = aid already received, 0 = no

	// ---------- Resources used by the current request ----------
	int used_trucks;
	int used_supply;
	string treatment;      // text describing the action taken

	// ---------- Counters for the final summary ----------
	int full_count = 0, standard_count = 0, emergency_count = 0, deferred_count = 0;

	cout << "==============================================\n";
	cout << "       RELIEF SUPPLY DISTRIBUTION SYSTEM\n";
	cout << "==============================================\n";
	cout << "Starting stock -> Trucks: " << num_trucks
		<< " | Supply Packs: " << num_supply << "\n";

	// ---------- Process the 5 requests ----------
	for (int i = 1; i <= 5; i++) {

		// Pick the name of the area for this request
		string request_name;
		switch (i) {
		case 1: request_name = "R1 - Riverside Village"; break;
		case 2: request_name = "R2 - Hilltop Settlement"; break;
		case 3: request_name = "R3 - Lakeside Camp"; break;
		case 4: request_name = "R4 - Market Town"; break;
		case 5: request_name = "R5 - Bridge Colony"; break;
		}

		cout << "\n----------------------------------------------\n";
		cout << " Enter details for " << request_name << "\n";
		cout << "----------------------------------------------\n";

		// ----- Input with validation: keep asking until the value is in range -----
		do {
			cout << "Damage level (1-Low, 2-Medium, 3-High): ";
			cin >> damage_level;
			if (damage_level < 1 || damage_level > 3)
				cout << "  Invalid input! Enter a value from 1 to 3.\n";
		} while (damage_level < 1 || damage_level > 3);

		do {
			cout << "Families affected (10-200): ";
			cin >> families;
			if (families < 10 || families > 200)
				cout << "  Invalid input! Enter a value from 10 to 200.\n";
		} while (families < 10 || families > 200);

		do {
			cout << "Is the route damaged? (1-Yes, 0-No): ";
			cin >> route;
			if (route < 0 || route > 1)
				cout << "  Invalid input! Enter 1 for Yes or 0 for No.\n";
		} while (route < 0 || route > 1);

		do {
			cout << "Is medical help needed? (1-Yes, 0-No): ";
			cin >> medical;
			if (medical < 0 || medical > 1)
				cout << "  Invalid input! Enter 1 for Yes or 0 for No.\n";
		} while (medical < 0 || medical > 1);

		do {
			cout << "Is there a flood warning? (1-Yes, 0-No): ";
			cin >> flood;
			if (flood < 0 || flood > 1)
				cout << "  Invalid input! Enter 1 for Yes or 0 for No.\n";
		} while (flood < 0 || flood > 1);

		do {
			cout << "Has aid been provided previously? (1-Yes, 0-No): ";
			cin >> previous_aid;
			if (previous_aid < 0 || previous_aid > 1)
				cout << "  Invalid input! Enter 1 for Yes or 0 for No.\n";
		} while (previous_aid < 0 || previous_aid > 1);

		// ----- Part A: calculate the need score -----
		int route_score = (route == 1) ? 15 : 0;
		int medical_score = (medical == 1) ? 12 : 0;
		int flood_score = (flood == 1) ? 30 : 0;
		int aid_score = (previous_aid == 1) ? -10 : 0;   // penalty

		// families / 5.0 (not 5) so the decimal part is not lost
		float score = (damage_level * 10) + (families / 5.0f)
			+ route_score + medical_score + flood_score + aid_score;

		cout << "\n>> " << request_name << "\n";
		cout << "   Need Score : " << fixed << setprecision(1) << score << "\n";

		// ----- Part B: decide the action based on score and remaining stock -----
		if (score >= 90) {
			// Highest priority: try Full, then fall back step by step
			if (num_trucks >= 2 && num_supply >= 50) {
				used_trucks = 2;  used_supply = 50;
				treatment = "Full Relief";
				full_count++;
			}
			else if (num_trucks >= 1 && num_supply >= 30) {
				used_trucks = 1;  used_supply = 30;
				treatment = "Downgraded to Standard Relief (insufficient stock for Full)";
				standard_count++;
			}
			else if (num_trucks >= 1 && num_supply >= 10) {
				used_trucks = 1;  used_supply = 10;
				treatment = "Downgraded to Emergency Kit (severely short on supplies)";
				emergency_count++;
			}
			else {
				used_trucks = 0;  used_supply = 0;
				treatment = "Deferred (out of supplies)";
				deferred_count++;
			}
		}
		else if (score >= 60) {
			// Medium-high priority: Standard, then Emergency
			if (num_trucks >= 1 && num_supply >= 30) {
				used_trucks = 1;  used_supply = 30;
				treatment = "Standard Relief";
				standard_count++;
			}
			else if (num_trucks >= 1 && num_supply >= 10) {
				used_trucks = 1;  used_supply = 10;
				treatment = "Downgraded to Emergency Kit (insufficient supplies)";
				emergency_count++;
			}
			else {
				used_trucks = 0;  used_supply = 0;
				treatment = "Deferred (out of supplies)";
				deferred_count++;
			}
		}
		else if (score >= 40) {
			// Medium priority: Emergency kit only
			if (num_trucks >= 1 && num_supply >= 10) {
				used_trucks = 1;  used_supply = 10;
				treatment = "Emergency Kit";
				emergency_count++;
			}
			else {
				used_trucks = 0;  used_supply = 0;
				treatment = "Deferred (out of supplies)";
				deferred_count++;
			}
		}
		else {
			// Low need: defer / monitor, no resources used
			used_trucks = 0;  used_supply = 0;
			treatment = "Deferred (low need score)";
			deferred_count++;
		}

		// ----- Update the remaining stock -----
		num_trucks -= used_trucks;
		num_supply -= used_supply;

		// ----- Show the result for this request -----
		cout << "   Treatment  : " << treatment << "\n";
		cout << "   Used       : " << used_trucks << " truck(s), " << used_supply << " pack(s)\n";
		cout << "   Remaining  : " << num_trucks << " truck(s), " << num_supply << " pack(s)\n";
	}

	// ---------- Final summary ----------
	cout << "\n==============================================\n";
	cout << "                   SUMMARY\n";
	cout << "==============================================\n";
	cout << left << setw(28) << "Full Relief provided:" << full_count << "\n";
	cout << left << setw(28) << "Standard Relief provided:" << standard_count << "\n";
	cout << left << setw(28) << "Emergency Kits provided:" << emergency_count << "\n";
	cout << left << setw(28) << "Deferred cases:" << deferred_count << "\n";
	cout << "----------------------------------------------\n";
	cout << left << setw(28) << "Trucks remaining:" << num_trucks << "\n";
	cout << left << setw(28) << "Supply packs remaining:" << num_supply << "\n";
	cout << "==============================================\n";

	return 0;
}