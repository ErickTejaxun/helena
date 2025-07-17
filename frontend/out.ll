; ModuleID = 'Helena'
source_filename = "Helena"

@0 = private unnamed_addr constant [8 x i8] c"Erickxx\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

declare i32 @printf(ptr, ...)

define i32 @main() {
entry:
  %cadena = alloca ptr, align 8
  store ptr @0, ptr %cadena, align 8
  %a = alloca i32, align 4
  store i32 111, ptr %a, align 4
  %b = alloca i32, align 4
  store i32 22, ptr %b, align 4
  %c = alloca i32, align 4
  store i32 12, ptr %c, align 4
  %0 = load ptr, ptr %cadena, align 8
  %calltmp = call i32 (ptr, ...) @printf(ptr @1, ptr %0)
  %1 = load i32, ptr %a, align 4
  %2 = load i32, ptr %c, align 4
  %3 = mul i32 %1, %2
  %4 = sub i32 1, %3
  ret i32 %4
}
