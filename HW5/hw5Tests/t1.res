@.Error division by zero = constant [18] x i8] c"div_by_zero_error\00"
@.%s\0A = constant [14] x i8] c"str_specifier\00"
@.%d\0A = constant [14] x i8] c"int_specifier\00"
@.var3 = constant [14] x i8] c""val is true"\00"
@.var4 = constant [15] x i8] c""val is false"\00"
@.var7 = constant [7] x i8] c""true"\00"
@.var8 = constant [7] x i8] c""true"\00"
declare i32 @printf(i8*, ...)
declare void @exit(i32)
define void @print(i8*) {
	call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @.str_specifier, i32 0, i32 0), i8* %0)
	ret void
}
define void @printi(i32) {
	call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @.int_specifier, i32 0, i32 0), i32 %0)
	ret void
}
%stack_array_ptr = alloca [50 x i32]
store i32 0, i32* %1
%var0 = getelementptr [50 x i32], [50 x i32]* stack_array_ptr, i32 0, i32 0
%var1 = load i32, i32* var0
%var2 = i32 %var1 to i1
br i1 %var2, label @, label @
call void @print(i8* getelementptr ([14 x i8], [14 x i8]* @.var3, i32 0, i32 0))
call void @print(i8* getelementptr ([15 x i8], [15 x i8]* @.var4, i32 0, i32 0))
%stack_array_ptr = alloca [50 x i32]
br label %label_20
label_20:
br label %label_24
label_22:
br label %label_24
label_24:
%var5 = phi i32 [1, %label_20], [0, %label_22]
call void @printByValue(i32 %var5)
br label %label_30
label_28:
br label %label_32
label_30:
br label %label_32
label_32:
%var6 = phi i32 [1, %label_28], [0, %label_30]
call void @printByValue(i32 %var6)
br label @
call void @print(i8* getelementptr ([7 x i8], [7 x i8]* @.var7, i32 0, i32 0))
br label %label_38
label_38:
br label @
label_40:
br label @
call void @print(i8* getelementptr ([7 x i8], [7 x i8]* @.var8, i32 0, i32 0))
