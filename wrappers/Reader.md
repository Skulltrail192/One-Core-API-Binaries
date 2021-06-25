About One-core-API

This is explanation about folders present on this.

api-sets: api-ms dlls present on WIn7+
applications: applications not present on XP/2003, like wusa
base: main wrappers, like kernelbase
dependecnies: dlls what wrappers need, it is not truely implementd, are so neeed to generate libs required to compile wrappers
directx: directx dx10+ based on wined3d implementation
drivers: special driver to boot animation
ex-set: like api-set, are present on Win7+
new-dlls: dlls what not exist on XP/2003
sdk: internal headers neeeded by wrappers and others, like directx dlls
todo:  To do tasks for One-core-API
to-synch: folders and files needed to one-core-api must be replace existing folder and files on Reactos repository base. You only copy the content inside to-synch and paste on reactos repositoy root