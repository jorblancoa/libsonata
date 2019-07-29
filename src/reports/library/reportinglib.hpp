#pragma once

#include <map>
#include <vector>
#include <string>

#ifdef HAVE_MPI
#include <mpi.h>
#endif

#include "report.hpp"

class ReportingLib {
    typedef std::map<std::string, std::shared_ptr<Report>> ReportMap;

  private:
    ReportMap m_reports;
    int m_numReports;

  public:
    static double m_atomicStep;
#ifdef HAVE_MPI
    static MPI_Comm m_allCells;
#endif
    static int m_rank;

    ReportingLib();
    ~ReportingLib();

    /**
     * Destroy all report objects.  This should invoke their destructor which will close the report
     * file and clean up
     */
    void clear();
    bool is_empty();

    int flush(double time);

    int get_num_reports();

    /**
     * Register a cell with a BinReport object.  Note that an earlier cell may have alreay created
     * the main Report object, so this
     * will simply add the cell to the end.
     *
     * @param report_name - Name of report, and key used to find the report
     * @param kind - The type of report ("compartment", "soma", "spike")
     */
    int add_report(const std::string& report_name, int cellnumber, unsigned long gid, unsigned long vgid,
                   double tstart, double tend, double dt,const std::string& kind);
    int add_variable(const std::string& report_name, int cellnumber, double* pointer);

    void make_global_communicator();
    void share_and_prepare();

    int record_data(double timestep, int ncells, int* cellids, const std::string& report_name);
    int end_iteration(double timestep);

    int set_max_buffer_size(const std::string& report_name, size_t buf_size);
    int set_max_buffer_size(size_t buf_size);

    Report::Kind string_to_enum(const std::string& kind);

};
