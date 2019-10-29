#pragma once

#include <map>
#include <vector>
#include <string>
#include <spdlog/spdlog.h>

#ifdef HAVE_MPI
#include <mpi.h>
#endif

#include "report.hpp"

/**
 *  \brief Contains and manages the reports
 */
class ReportingLib {
    typedef std::map<std::string, std::shared_ptr<Report>> reports_t;
#ifdef HAVE_MPI
    typedef std::map<std::string, MPI_Comm> communicators_t;
#endif
  private:
    reports_t m_reports;
    int m_num_reports;

  public:
    static double m_atomic_step;
    static double m_min_steps_to_record;
#ifdef HAVE_MPI
    static MPI_Comm m_has_nodes;
    static communicators_t m_communicators;
#endif
    static int m_rank;
    static bool first_report;

    ReportingLib();
    ~ReportingLib();

    /**
     * \brief Destroy all report objects.
     * This should invoke their destructor which will close the report
     * file and clean up
     */
    void clear();
    bool is_empty();

    int flush(double time);
    void refresh_pointers(refresh_function_t refresh_function);

    int get_num_reports() const;

    /**
     * \brief Register a node with a BinReport object.
     * Note that an earlier node may have already created
     * the main Report object, so this
     * will simply add the node to the end.
     *
     * @param report_name - Name of report, and key used to find the report
     * @param kind - The type of report ("compartment", "soma")
     */
    int add_report(const std::string& report_name, uint64_t node_id, uint64_t gid, uint64_t vgid,
                   double tstart, double tend, double dt, const std::string& kind);
    int add_variable(const std::string& report_name, uint64_t node_id, double* voltage, uint32_t element_id);

    void make_global_communicator();
    void prepare_datasets();

    int record_nodes_data(double step, const std::vector<uint64_t>& node_ids, const std::string& report_name);
    int record_data(double step);
    int end_iteration(double timestep);

    int set_max_buffer_size(const std::string& report_name, size_t buf_size);
    int set_max_buffer_size(size_t buffer_size);
    void write_spikes(const std::vector<double>& spike_timestamps, const std::vector<int>& spike_node_ids);
};
