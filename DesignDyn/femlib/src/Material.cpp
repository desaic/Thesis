#include "Material.hpp"
#include "MatrixX.hpp"
Material::Material(){}
Material::~Material(){}
MatrixXf Material::getStiffness(Element* ele, ElementMesh * mesh)
{
  return MatrixXf();
}