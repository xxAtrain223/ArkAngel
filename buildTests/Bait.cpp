#include "bait_static.hpp"
#include "bait_dynamic.hpp"

#include "bait_print_static.hpp"
#include "bait_print_dynamic.hpp"

#include <iostream>

using namespace std;

#if 1
using BT = bait::DynamicBT<>;
template <typename... Ts>
void print(Ts&&... ts) {return bait::print_dynamic(forward<Ts>(ts)...);}
#else
using BT = bait::StaticBT;
template <typename... Ts>
void print(Ts&&... ts) {return bait::print_static(forward<Ts>(ts)...);}
#endif

constexpr auto simplify = bait::Simplifier<BT,bait::Optimization::ALL>();

using bait::status;
status find_player() { return status::SUCCESS; }
status player_in_range() { return status::SUCCESS; }
status player_out_range() { return status::SUCCESS; }
status attack() { return status::SUCCESS; }
status walk_toward_player() { return status::SUCCESS; }
status walk_randomly() { return status::SUCCESS; }
status stand() { return status::SUCCESS; }

auto behavior =
    BT::sequence(
        BT::sequence(
            BT::until_fail(
                find_player
            ),
            BT::selector(
                BT::sequence(
                    player_in_range,
                    BT::inverter(BT::inverter(attack))
                ),
                walk_toward_player,
                BT::selector(
                    BT::selector(
                        player_out_range
                    )
                )
            )
        ),
        BT::until_fail(
            BT::selector(
                BT::inverter(
                    BT::sequence(
                        walk_randomly
                    )
                ),
                stand
            )
        ),
        BT::selector(
            BT::inverter(find_player),
            BT::inverter(walk_randomly),
            BT::inverter(stand)
        )
    );
auto behavior2 = simplify(behavior);

int main() {
    cout << "BEFORE SIMPLIFY:" << endl;
    print(cout, behavior, "    ");
    cout << "AFTER SIMPLIFY:" << endl;
    print(cout, behavior2, "    ");

    using strings = string[3];
    cout << "Result: " << (strings{"Success!", "Failure!", "Running!"}[(int) behavior2()]) << endl;

    cout << "Sizeof behavior:  " << sizeof(behavior) << endl;
    cout << "Sizeof behavior2: " << sizeof(behavior2) << endl;
}
