// parsing an expression with wrong number of components


#include "tests.h"
#include "utilities.h"
#include "parameter_acceptor.h"
#include "parsed_mapped_functions.h"


int main ()
{
  initlog();
  ParsedMappedFunctions<2,3> pmf("Dirichlet BCs", "u,u,p",
                                 "0=u , 1=1 , 6=u;p",
                                 "0=x;y;0 , 1=0;0;0;0 , 6=y*k;0;k","k=1");

  ParameterAcceptor::initialize(SOURCE_DIR "/parameters/parsed_mapped_functions_06.prm", "used_parameters.prm");
  ParameterAcceptor::prm.log_parameters(deallog);

  Point<2> p(2,3);

  std::vector<unsigned int> ids = pmf.get_mapped_ids();
  for (unsigned int i=0; i<ids.size(); ++i)
    {
      deallog << "Component mask id " << ids[i] <<" "<<  pmf.get_mapped_mask(ids[i]) << std::endl;
      deallog << "Parsed Function on id: "<< ids[i] <<" "<<  (*pmf.get_mapped_function(ids[i])).value(p) << std::endl;
    }
}
