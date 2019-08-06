#include <catch2/catch.hpp>
#include <reports/library/report.hpp>
#include <reports/library/element_report.hpp>
#include <reports/library/soma_report.hpp>
#include <reports/library/spike_report.hpp>
#include <memory>

SCENARIO( "Test Report class", "[Report]" ) {
    const double tstart = 0;
    const double tend = 1.0;
    const double dt = 0.1;
    uint32_t soma_id = 42;
    uint32_t element_id = 142;
    GIVEN( "An instance of a soma report" ) {
        std::shared_ptr<Report> soma_report = std::make_shared<SomaReport>("somaReport", tstart, tend, dt);
        WHEN("We add a node and a variable to a soma report") {
            soma_report->add_node(1,1,1);
            double element_value = 10;
            soma_report->add_variable(1, &element_value, soma_id);
            THEN("Number of nodes and elements is 1") {
                REQUIRE(soma_report->get_num_nodes() == 1);
                REQUIRE(soma_report->get_total_elements() == 1);
            }
        }
        WHEN("We add 2 elements to a given node in a soma report") {
            soma_report->add_node(1,1,1);
            double element_value = 42;
            soma_report->add_variable(1, &element_value, soma_id);
            //soma_report->add_variable(1, &element_value);
            REQUIRE_THROWS(soma_report->add_variable(1, &element_value, soma_id));
            THEN("Number of nodes and elements is still 1") {
                REQUIRE(soma_report->get_num_nodes() == 1);
                REQUIRE(soma_report->get_total_elements() == 1);
            }
        }
    }

    GIVEN( "An instance of a element report" ) {
        std::shared_ptr<Report> element_report = std::make_shared<ElementReport>("elementReport", tstart, tend, dt);
        WHEN("We add 2 nodes") {
            element_report->add_node(1,1,1);
            element_report->add_node(2,2,2);

            THEN("Number of nodes is 2") {
                REQUIRE(element_report->get_num_nodes() == 2);
            }
        }

        WHEN("We add 10 elements") {
            element_report->add_node(1,1,1);
            element_report->add_node(2,2,2);
            double element_value = 10;
            element_report->add_variable(1, &element_value, element_id);

            std::vector<double> voltages = { 1, 20, 300, 4000, 500, 60, 7, 0.8, 9 };
            for(auto& voltage: voltages) {
                element_report->add_variable(2, &voltage, element_id);
            }

            // Doesn't exists
            REQUIRE_THROWS(element_report->add_variable(3, &element_value, element_id));
            THEN("Number of nodes is 2") {
                REQUIRE(element_report->get_num_nodes() == 2);
            }
            THEN("The number of elements is 6") {
                REQUIRE(element_report->get_total_elements() == 10);
            }
        }
    }

    GIVEN( "An instance of a spike report" ) {
        std::shared_ptr<Report> spike_report = std::make_shared<SpikeReport>("spikeReport", tstart, tend, dt);
        WHEN("We try to add 2 nodes with same id to a spike report") {
            spike_report->add_node(1,1,1);
            REQUIRE_THROWS(spike_report->add_node(1,2,2));
            THEN("Number of nodes is still 1 (ignoring second insert)") {
                REQUIRE(spike_report->get_num_nodes() == 1);
            }
        }
    }
}
