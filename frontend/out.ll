; ModuleID = 'Helena'
source_filename = "Helena"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 111, ptr %a, align 4
  %b = alloca i32, align 4
  store i32 22, ptr %b, align 4
  %c = alloca i32, align 4
  store i32 12, ptr %c, align 4
  ret i32 0
}

define i32 @suma() {
entry:
  %d = alloca i32, align 4
  store i32 13, ptr %d, align 4
  %0 = load ptr, ptr %d, align 8
  %loadtmp = load i32, ptr %0, align 4
  ret i32 %loadtmp
}

define double @sss() {
entry:
  %e = alloca double, align 8
  store double 1.400000e+01, ptr %e, align 8
  %0 = load ptr, ptr %e, align 8
  %loadtmp = load double, ptr %0, align 8
  ret double %loadtmp
}
